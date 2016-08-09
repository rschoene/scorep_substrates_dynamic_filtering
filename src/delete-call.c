#define UNW_LOCAL_ONLY
#define _BSD_SOURCE

#include <libunwind.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

//#include <UTILS_Error.h>

#include "delete-call.h"

/**
   Helper function for scorep_filtering_override_callq. Writes a five byte NOP to the given position.
   @param ptr       Position to write the NOP.
 */
static void
write_nop( char* ptr )
{
    memset( ptr,     0x0f, 1 );
    memset( ptr + 1, 0x1f, 1 );
    memset( ptr + 2, 0x44, 1 );
    memset( ptr + 3, 0x00, 1 );
    memset( ptr + 4, 0x00, 1 );
}

/**
   Helper function for scorep_filtering_override_callq. Changes the memory access rights and adds or
   removes the write right.
   @param ptr       Memory position to make (not) writeable.
   @param writable  Whether or not to make the memory writeable (1) or not (0).
 */
static void
change_memory_access_rights( char* ptr, const int writeable )
{
    int page_size = getpagesize( );
    void* first_ptr = ptr - ( (unsigned long) ptr % page_size );
    void* second_ptr = ( ptr + 4 ) - ( (unsigned long) ( ptr + 4 ) % page_size );

    if( writeable == 1 )
    {
        if( mprotect( first_ptr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC ) != 0
         || mprotect( second_ptr, page_size, PROT_READ | PROT_WRITE | PROT_EXEC ) != 0 )
        {
            //UTILS_FATAL( "Could not add write right to memory access rights on position %#010x",
                         //ptr );
        }
    }
    else
    {
        if( mprotect( first_ptr, page_size, PROT_READ | PROT_EXEC ) != 0
         || mprotect( second_ptr, page_size, PROT_READ | PROT_EXEC ) != 0 )
        {
            //UTILS_FATAL( "Could not remove write right from memory access rights on position %#010x",
                         //ptr );
        }
    }
}

/**
   Overrides a callq at the given position with a five byte NOP.
   @param ptr       Position of the callq to override.
 */
void override_callq( char* ptr )
{
    change_memory_access_rights( ptr, 1 );
    write_nop( ptr );
    change_memory_access_rights( ptr, 0 );
}

char* get_function_call_ip( const char* function_name )
{
    printf( "In SCOREP_DeleteCall, name = %s\n", function_name );
    unw_cursor_t cursor;
    unw_context_t uc;
    unw_word_t ip, offset;
    char sym[256];

    unw_getcontext( &uc );
    unw_init_local( &cursor, &uc );

    do
    {
        if( unw_get_proc_name( &cursor, sym, sizeof( sym ), &offset ) == 0
            && strcmp( sym, function_name ) == 0 )
        {
            unw_step( &cursor );
            unw_get_reg( &cursor, UNW_REG_IP, &ip );

            return (char*) ( ip - 5 );
        }
    } while( unw_step( &cursor ) > 0 );

    // This shouldn't happen
    return (char*) -1;
}
