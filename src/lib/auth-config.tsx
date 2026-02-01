"use client";

import { createContext, useContext, ReactNode } from "react";

const AuthConfigContext = createContext(false);

export function AuthConfigProvider({ hasClerk, children }: { hasClerk: boolean; children: ReactNode }) {
  return <AuthConfigContext.Provider value={hasClerk}>{children}</AuthConfigContext.Provider>;
}

export function useAuthConfig() {
  return useContext(AuthConfigContext);
}
