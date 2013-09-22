/*
===========================================================================
Copyright (C) 1999-2005 Id Software, Inc.
Copyright (C) 2000-2006 Tim Angus

This file is part of Tremulous.

Tremulous is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 2 of the License,
or (at your option) any later version.

Tremulous is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Tremulous; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
===========================================================================
*/

// this file holds commands that can be executed by the server console, but not remote clients

#include "g_local.h"

/*
===================
Svcmd_EntityList_f
===================
*/
void  Svcmd_EntityList_f( void )
{
  int       e;
  gentity_t *check;

  check = g_entities;

  for( e = 0; e < level.num_entities; e++, check++ )
  {
    if( !check->inuse )
      continue;

    G_Printf( "%3i:", e );

    switch( check->s.eType )
    {
      case ET_GENERAL:
        G_Printf( "ET_GENERAL          " );
        break;
      case ET_PLAYER:
        G_Printf( "ET_PLAYER           " );
        break;
      case ET_ITEM:
        G_Printf( "ET_ITEM             " );
        break;
      case ET_BUILDABLE:
        G_Printf( "ET_BUILDABLE        " );
        break;
      case ET_MISSILE:
        G_Printf( "ET_MISSILE          " );
        break;
      case ET_MOVER:
        G_Printf( "ET_MOVER            " );
        break;
      case ET_BEAM:
        G_Printf( "ET_BEAM             " );
        break;
      case ET_PORTAL:
        G_Printf( "ET_PORTAL           " );
        break;
      case ET_SPEAKER:
        G_Printf( "ET_SPEAKER          " );
        break;
      case ET_PUSH_TRIGGER:
        G_Printf( "ET_PUSH_TRIGGER     " );
        break;
      case ET_TELEPORT_TRIGGER:
        G_Printf( "ET_TELEPORT_TRIGGER " );
        break;
      case ET_INVISIBLE:
        G_Printf( "ET_INVISIBLE        " );
        break;
      case ET_GRAPPLE:
        G_Printf( "ET_GRAPPLE          " );
        break;
      default:
        G_Printf( "%3i                 ", check->s.eType );
        break;
    }

    if( check->classname )
      G_Printf( "%s", check->classname );

    G_Printf( "\n" );
  }
}

static gclient_t *ClientForString( char *s )
{
  int  idnum;

  idnum = G_ClientNumberFromString( s );
  if( idnum == -1 )
  {
    G_Printf( "%s", "Client not found\n" );
    return NULL;
  }

  return &level.clients[ idnum ];
}

/*
===================
Svcmd_LayoutSave_f

layoutsave <name>
===================
*/
void  Svcmd_LayoutSave_f( void )
{
  char str[ MAX_QPATH ];
  char str2[ MAX_QPATH - 4 ];
  char *s;
  int i = 0;

  if( trap_Argc( ) != 2 )
  {
    G_Printf( "usage: layoutsave <name>\n" );
    return;
  }
  trap_Argv( 1, str, sizeof( str ) );

  // sanitize name
  s = &str[ 0 ];
  while( *s && i < sizeof( str2 ) - 1 )
  {
    if( isalnum( *s ) || *s == '-' || *s == '_' )
    {
      str2[ i++ ] = *s;
      str2[ i ] = '\0';
    }
    s++;
  }

  if( !str2[ 0 ] )
  {
    G_Printf("layoutsave: invalid name \"%s\"\n", str );
    return;
  }

  G_LayoutSave( str2 );
}

char  *ConcatArgs( int start );

/*
===================
Svcmd_LayoutLoad_f

layoutload [<name> [<name2> [<name3 [...]]]]

This is just a silly alias for doing:
 set g_layouts "name name2 name3"
 map_restart
===================
*/
void  Svcmd_LayoutLoad_f( void )
{
  char layouts[ MAX_CVAR_VALUE_STRING ];
  char *s;

  s = ConcatArgs( 1 );
  Q_strncpyz( layouts, s, sizeof( layouts ) );
  trap_Cvar_Set( "g_layouts", layouts ); 
  trap_SendConsoleCommand( EXEC_APPEND, "map_restart\n" );
  level.restarted = qtrue;
}

static void Svcmd_AdmitDefeat_f( void )
{
  int  team;
  char teamNum[ 2 ];

  if( trap_Argc( ) != 2 )
  {
    G_Printf("admitdefeat: must provide a team\n");
    return;
  }
  trap_Argv( 1, teamNum, sizeof( teamNum ) );
  team = atoi( teamNum );
  if( team == PTE_ALIENS || teamNum[ 0 ] == 'a' )
  {
    level.surrenderTeam = PTE_ALIENS;
    G_BaseSelfDestruct( PTE_ALIENS );
    G_TeamCommand( PTE_ALIENS, "cp \"Hivemind Link Broken\" 1");
    trap_SendServerCommand( -1, "print \"Alien team has admitted defeat\n\"" );
  }
  else if( team == PTE_HUMANS || teamNum[ 0 ] == 'h' )
  {
    level.surrenderTeam = PTE_HUMANS;
    G_BaseSelfDestruct( PTE_HUMANS );
    G_TeamCommand( PTE_HUMANS, "cp \"Life Support Terminated\" 1");
    trap_SendServerCommand( -1, "print \"Human team has admitted defeat\n\"" );
  }
  else
  {
    G_Printf("admitdefeat: invalid team\n");
  } 
}

static void Svcmd_Evacuation_f( void )
{
    trap_SendServerCommand( -1, "print \"Evacuation ordered\n\"" );
    level.lastWin = PTE_NONE;
    trap_SetConfigstring( CS_WINNER, "Evacuation" );
    LogExit( "Evacuation." );
    G_admin_maplog_result( "d" );
}

static void Svcmd_AlienWin_f( void )
{
    int i;
    gentity_t *e;

    for( i = 1, e = g_entities + i; i < level.num_entities; i++, e++ )
    {
      if( e->s.modelindex == BA_H_SPAWN )
        G_Damage( e, NULL, NULL, NULL, NULL, 10000, 0, MOD_SUICIDE );
    }
}

static void Svcmd_HumanWin_f( void )
{
    int i;
    gentity_t *e;

    for( i = 1, e = g_entities + i; i < level.num_entities; i++, e++ )
    {
      if( e->s.modelindex == BA_A_SPAWN )
        G_Damage( e, NULL, NULL, NULL, NULL, 10000, 0, MOD_SUICIDE );
    }
}

static void Svcmd_CenterPrint_f( void )
{
    char buffer[MAX_STRING_CHARS];

    Q_strncpyz( buffer, ConcatArgs( 1 ), sizeof( buffer ) );
    G_ParseEscapedString( buffer );
    trap_SendServerCommand( -1, va( "cp \"%s\"", buffer ) );
    trap_SendServerCommand( -1, va( "print \"CP: %s\n\"", buffer ) );
    G_Printf( "cp: %s\n", ConcatArgs( 1 ) );
}

static void Svcmd_MessageWrapper( void )
{
  char cmd[ 5 ];
  trap_Argv( 0, cmd, sizeof( cmd ) );

  if( !Q_stricmp( cmd, "a" ) || !Q_stricmp( cmd, "say_admins") )
    G_Say( NULL, NULL, SAY_ADMINS, ConcatArgs( 1 )  );
  else if( !Q_stricmp( cmd, "m" ) )
    G_PrivateMessage( NULL );
  else if( !Q_stricmp( cmd, "say" ) )
    trap_SendServerCommand( -1, va( "print \"server: %s\n\"", ConcatArgs( 1 ) ) );
  else if( !Q_stricmp( cmd, "chat" ) )
  {
    trap_SendServerCommand( -1, va( "chat \"%s\" -1 0", ConcatArgs( 1 ) ) );
    G_Printf( "chat: %s\n", ConcatArgs( 1 ) );
  }
}

static void Svcmd_MapRotation_f( void )
{
  char rotationName[ MAX_QPATH ];

  if( trap_Argc( ) != 2 )
  {
    G_Printf( "usage: maprotation <name>\n" );
    return;
  }

  trap_Argv( 1, rotationName, sizeof( rotationName ) );
  if( !G_StartMapRotation( rotationName, qfalse ) )
    G_Printf( "maprotation: invalid map rotation \"%s\"\n", rotationName );
}

static void Svcmd_Status_f( void )
{
  int       i;
  gclient_t *cl;
  char      userinfo[ MAX_INFO_STRING ];

  G_Printf( "slot score ping address               rate     name\n" );
  G_Printf( "---- ----- ---- -------               ----     ----\n" );
  for( i = 0, cl = level.clients; i < level.maxclients; i++, cl++ )
  {
    if( cl->pers.connected == CON_DISCONNECTED )
      continue;

    G_Printf( "%-4d ", i );
    G_Printf( "%-5d ", cl->ps.persistant[ PERS_SCORE ] );

    if( cl->pers.connected == CON_CONNECTING )
      G_Printf( "CNCT " );
    else
      G_Printf( "%-4d ", cl->ps.ping );

    trap_GetUserinfo( i, userinfo, sizeof( userinfo ) );
    G_Printf( "%-21s ", Info_ValueForKey( userinfo, "ip" ) );
    G_Printf( "%-8s ", Info_ValueForKey( userinfo, "rate" ) );
    G_Printf( "%s\n", cl->pers.netname );
  }
}

static void Svcmd_DumpUser_f( void )
{
  char name[ MAX_STRING_CHARS ], userinfo[ MAX_INFO_STRING ];
  char key[ BIG_INFO_KEY ], value[ BIG_INFO_VALUE ];
  const char *info;
  gclient_t *cl;

  if( trap_Argc( ) != 2 )
  {
    G_Printf( "usage: dumpuser <player>\n" );
    return;
  }

  trap_Argv( 1, name, sizeof( name ) );
  cl = ClientForString( name );
  if( !cl )
    return;

  trap_GetUserinfo( cl-level.clients, userinfo, sizeof( userinfo ) );
  info = &userinfo[ 0 ];
  G_Printf( "userinfo\n--------\n" );

  while( 1 )
  {
    Info_NextPair( &info, key, value );
    if( !*info )
      return;

    G_Printf( "%-20s%s\n", key, value );
  }
}

struct svcmd
{
  char     *cmd;
  qboolean dedicated;
  void     ( *function )( void );
} svcmds[ ] = {
    { "a", qtrue, Svcmd_MessageWrapper },
    { "admitdefeat", qfalse, Svcmd_AdmitDefeat_f },
    { "advanceMapRotation", qfalse, G_AdvanceMapRotation },
    { "alienWin", qfalse, Svcmd_AlienWin_f },
    { "chat", qtrue, Svcmd_MessageWrapper },
    { "cp", qtrue, Svcmd_CenterPrint_f },
    { "dumpuser", qfalse, Svcmd_DumpUser_f },
    { "entitylist", qfalse, Svcmd_EntityList_f },
    { "evacuation", qfalse, Svcmd_Evacuation_f },
    { "game_memory", qfalse, Svcmd_GameMem_f },
    { "humanWin", qfalse, Svcmd_HumanWin_f },
    { "layoutload", qfalse, Svcmd_LayoutLoad_f },
    { "layoutsave", qfalse, Svcmd_LayoutSave_f },
    { "m", qtrue, Svcmd_MessageWrapper },
    { "mapRotation", qfalse, Svcmd_MapRotation_f },
    { "nobuildload", qfalse, G_NobuildLoad },
    { "nobuildsave", qfalse, G_NobuildSave },
    { "say", qtrue, Svcmd_MessageWrapper },
    { "say_admins", qtrue, Svcmd_MessageWrapper },
    { "status", qfalse, Svcmd_Status_f },
    { "stopMapRotation", qfalse, G_StopMapRotation }
};

/*
=================
ConsoleCommand

=================
*/
qboolean ConsoleCommand( void )
{
  char cmd[ MAX_TOKEN_CHARS ];
  struct svcmd *command;

  trap_Argv( 0, cmd, sizeof( cmd ) );

  command = bsearch( cmd, svcmds, sizeof( svcmds ) / sizeof( struct svcmd ),
    sizeof( struct svcmd ), cmdcmp );

  if( !command )
  {
    // see if this is an admin command
    if( G_admin_cmd_check( NULL, qfalse ) )
      return qtrue;

    if( g_dedicated.integer )
      G_Printf( "unknown command: %s\n", cmd );

    return qfalse;
  }

  if( command->dedicated && !g_dedicated.integer )
    return qfalse;

  command->function( );
  return qtrue;
}
