$ErrorActionPreference = "Stop"

$repositoryRoot = Split-Path -Parent $PSScriptRoot
$lockPath = Join-Path $repositoryRoot "pawn.lock"

Push-Location $repositoryRoot
try {
	& sampctl ensure --lock-only
	if ($LASTEXITCODE -ne 0) {
		throw "Could not resolve dependency revisions for pawn.lock."
	}
}
finally {
	Pop-Location
}

$lock = Get-Content -LiteralPath $lockPath -Raw | ConvertFrom-Json
if ($lock.version -ne 1) {
	throw "Unsupported pawn.lock version: $($lock.version)"
}

$dependencies = @($lock.dependencies.PSObject.Properties)
if ($dependencies.Count -eq 0) {
	throw "The generated pawn.lock contains no dependencies."
}

foreach ($dependency in $dependencies) {
	if ($dependency.Value.commit -notmatch "^[0-9a-f]{40}$") {
		throw "Dependency $($dependency.Name) does not have a full commit pin."
	}
	$null = $dependency.Value.PSObject.Properties.Remove("integrity")
}

$serializedLock = $lock | ConvertTo-Json -Depth 100
[System.IO.File]::WriteAllText(
	$lockPath,
	$serializedLock + [Environment]::NewLine,
	[System.Text.UTF8Encoding]::new($false)
)

Write-Output "Updated pawn.lock with $($dependencies.Count) pinned dependency commits."
