import { clerkMiddleware, createRouteMatcher } from '@clerk/nextjs/server'
import { NextResponse } from 'next/server'
import type { NextRequest } from 'next/server'

const isProtectedRoute = createRouteMatcher(['/dashboard(.*)', '/admin(.*)'])
const isPublicRoute = createRouteMatcher(['/sign-in(.*)', '/sign-up(.*)', '/', '/purchase'])

const hasClerkKeys = !!(
  process.env.NEXT_PUBLIC_CLERK_PUBLISHABLE_KEY?.startsWith('pk_') &&
  process.env.CLERK_SECRET_KEY?.startsWith('sk_')
)

export default hasClerkKeys
  ? clerkMiddleware(async (auth, req) => {
      if (!isPublicRoute(req) && isProtectedRoute(req)) await auth.protect()
    })
  : (req: NextRequest) => {
      if (isProtectedRoute(req) && !isPublicRoute(req)) {
        return NextResponse.redirect(new URL('/sign-in', req.url))
      }
      return NextResponse.next()
    }

export const config = {
  matcher: [
    '/((?!_next|[^?]*\\.(?:html?|css|js(?!on)|jpe?g|webp|png|gif|svg|ttf|woff2?|ico|csv|docx?|xlsx?|zip|webmanifest)).*)',
    '/(api|trpc)(.*)',
  ],
}
