ls

$OutputEncoding = New-Object -typename System.Text.UTF8Encoding
chcp 65001
$OutputEncoding

git clone -q --depth=1 https://github.com/martinrotter/rssguard.wiki.git C:\rssguard-wiki
git config --global credential.helper store
Add-Content "$env:USERPROFILE\.git-credentials" "https://$($env:access_token):x-oauth-basic@github.com`n"
git config --global user.email "rotter.martinos@gmail.com"
git config --global user.name "martinrotter"

$git_revision = git rev-parse --short HEAD
$date = (Get-Date).ToUniversalTime().ToString("MM-dd-yyyy HH:mm:ss UTC")
$wikifile = 'C:\rssguard-wiki\Development-builds.md'


$file = (Get-ChildItem '*.7z').Name
echo "File to upload: $file"
$url = curl.exe --upload-file "$file" "https://transfer.sh/$file" --silent
echo "Obtained URL: $url"

$webengine_type = if ($file -like '*nowebengine*') { echo "false" } else { echo "true" }
$regex = "\| Windows \|.+transfer\.sh \(7z\).+ $webengine_type \|  "
$wikiline = "| Windows | $date | [$git_revision](https://github.com/martinrotter/rssguard/commit/$git_revision) | [transfer.sh (7z)]($url) | $webengine_type |  "

(Get-Content $wikifile) -replace $regex, $wikiline | Set-Content -Encoding "utf8" $wikifile


$file = (Get-ChildItem '*.exe').Name
echo "File to upload: $file"
$url = curl.exe --upload-file "$file" "https://transfer.sh/$file" --silent
echo "Obtained URL: $url"

$webengine_type = if ($file -like '*nowebengine*') { echo "false" } else { echo "true" }
$regex = "\| Windows \|.+transfer\.sh \(exe\).+ $webengine_type \|  "
$wikiline = "| Windows | $date | [$git_revision](https://github.com/martinrotter/rssguard/commit/$git_revision) | [transfer.sh (exe)]($url) | $webengine_type |  "

(Get-Content $wikifile) -replace $regex, $wikiline | Set-Content -Encoding "utf8" $wikifile


cd C:\rssguard-wiki
git add *.*
git commit -m "New files."
git pull origin master
git push origin master -v