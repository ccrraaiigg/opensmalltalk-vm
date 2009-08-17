CHECK_FUNCTION_EXISTS (unsetenv HAVE_UNSETENV)
CHECK_LIBRARY_EXISTS (pthread pthread_kill "" HAVE_LIB_PTHREAD)

IF (NOT HAVE_LIB_PTHREAD)
    DISABLE_PLUGIN ()
ENDIF ()

CONFIG_DEFINE (HAVE_UNSETENV)