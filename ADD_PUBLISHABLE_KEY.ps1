# Add NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY to Vercel
# Get pk_test_... from https://dashboard.clerk.com → API Keys → Publishable
$pk = Read-Host "Paste your Clerk Publishable Key (pk_test_...)"
if ($pk -match '^pk_') {
  Write-Host "Adding to Vercel production..."
  $pk | vercel env add NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY production
  Write-Host "Adding to Vercel preview..."
  $pk | vercel env add NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY preview
  Write-Host "Adding to Vercel development..."
  $pk | vercel env add NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY development
  Write-Host "Done. Redeploy from Vercel dashboard or push to GitHub."
} else {
  Write-Host "Key should start with pk_test_ or pk_live_"
}
