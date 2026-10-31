$ErrorActionPreference = "Stop"

$repositoryRoot = Split-Path -Parent $PSScriptRoot
$dependencyPath = Join-Path $repositoryRoot "dependencies\gBugFix"
$repositoryUrl = "https://github.com/ZantetsukenGT/gBugFix.git"
$expectedCommit = "44c01f419c197a1b7927c2bd4ecc7f253faac325"

if (-not (Test-Path -LiteralPath $dependencyPath)) {
	& git clone $repositoryUrl $dependencyPath
	if ($LASTEXITCODE -ne 0) {
		throw "Could not clone gBugFix."
	}
}
elseif (-not (Test-Path -LiteralPath (Join-Path $dependencyPath ".git"))) {
	throw "The gBugFix path exists but is not a Git checkout."
}

$currentCommit = (& git -C $dependencyPath rev-parse HEAD).Trim()
if ($LASTEXITCODE -ne 0) {
	throw "Could not read the gBugFix revision."
}

if ($currentCommit -ne $expectedCommit) {
	$worktreeChanges = & git -C $dependencyPath status --porcelain
	if ($LASTEXITCODE -ne 0) {
		throw "Could not inspect the gBugFix worktree."
	}
	if ($worktreeChanges) {
		throw "gBugFix has local changes; refusing to switch revisions."
	}

	& git -C $dependencyPath fetch origin $expectedCommit
	if ($LASTEXITCODE -ne 0) {
		throw "Could not fetch the pinned gBugFix revision."
	}
	& git -C $dependencyPath checkout --detach $expectedCommit
	if ($LASTEXITCODE -ne 0) {
		throw "Could not check out the pinned gBugFix revision."
	}
}

Write-Output "Prepared gBugFix at $expectedCommit."
