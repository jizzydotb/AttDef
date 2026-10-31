$ErrorActionPreference = "Stop"

$repositoryRoot = Split-Path -Parent $PSScriptRoot

function Prepare-DependencyRepo {
	param(
		[string]$RepoName,
		[string]$ExpectedCommit,
		[string]$PatchFile
	)

	$dependencyPath = Join-Path $repositoryRoot "dependencies\$RepoName"
	$patchPath = Join-Path $repositoryRoot "patches\$PatchFile"

	if (-not (Test-Path -LiteralPath (Join-Path $dependencyPath ".git"))) {
		throw "$RepoName checkout is missing. Run sampctl ensure first."
	}

	$currentCommit = (& git -C $dependencyPath rev-parse HEAD).Trim()
	if ($LASTEXITCODE -ne 0) {
		throw "Could not read the $RepoName revision."
	}

	if ($currentCommit -ne $ExpectedCommit) {
		& git -C $dependencyPath fetch origin $ExpectedCommit
		if ($LASTEXITCODE -ne 0) {
			throw "Could not fetch the pinned $RepoName revision ($ExpectedCommit)."
		}
		& git -C $dependencyPath checkout --force --detach $ExpectedCommit
		if ($LASTEXITCODE -ne 0) {
			throw "Could not check out the pinned $RepoName revision ($ExpectedCommit)."
		}
	}

	$null = & git -C $dependencyPath apply --reverse --check --ignore-space-change --ignore-whitespace --unidiff-zero $patchPath 2>$null
	if ($LASTEXITCODE -eq 0) {
		Write-Output "$RepoName project patch is already applied."
		return
	}

	& git -C $dependencyPath apply --check --ignore-space-change --ignore-whitespace --unidiff-zero $patchPath
	if ($LASTEXITCODE -ne 0) {
		throw "$RepoName patch does not apply cleanly to the pinned revision."
	}
	& git -C $dependencyPath apply --ignore-space-change --ignore-whitespace --unidiff-zero $patchPath
	if ($LASTEXITCODE -ne 0) {
		throw "Could not apply the $RepoName patch."
	}

	Write-Output "Prepared $RepoName at $ExpectedCommit with the project patch."
}

Prepare-DependencyRepo -RepoName "samp-weapon-config" -ExpectedCommit "c7512b7a4e2a8412fe9ff14337ef8924e8ff74cb" -PatchFile "samp-weapon-config-healthbar.patch"
Prepare-DependencyRepo -RepoName "Pawn.RakNet"        -ExpectedCommit "dacb88d1c05d56614e6d2c6cd760d7dd6bba0df8" -PatchFile "Pawn.RakNet.patch"
Prepare-DependencyRepo -RepoName "samp-geoip"         -ExpectedCommit "1e8534ee8e76d526a3710fca44992dd75867032b" -PatchFile "samp-geoip.patch"
Prepare-DependencyRepo -RepoName "sscanf"             -ExpectedCommit "27f0b725095bf40ed9c4d772acd7300b02be632b" -PatchFile "sscanf.patch"

$dccTarget = Join-Path $repositoryRoot "dependencies\discord-connector.inc"
if (-not (Test-Path -LiteralPath $dccTarget)) {
	$dccSource = Get-ChildItem -Path (Join-Path $repositoryRoot "dependencies\.resources") -Filter "discord-connector.inc" -Recurse -ErrorAction SilentlyContinue | Select-Object -First 1
	if ($dccSource) {
		Copy-Item -LiteralPath $dccSource.FullName -Destination $dccTarget -Force
		Write-Output "Copied discord-connector.inc into dependencies/."
	}
}
