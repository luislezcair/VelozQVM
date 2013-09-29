/*
===========================================================================
Copyright (C) 2004-2006 Tony J. White

This file is part of Tremulous.

This shrubbot implementation is the original work of Tony J. White.

Contains contributions from Wesley van Beelen, Chris Bajumpaa, Josh Menke,
and Travis Maurer.

The functionality of this code mimics the behaviour of the currently
inactive project shrubet (http://www.etstats.com/shrubet/index.php?ver=2)
by Ryan Mannion.   However, shrubet was a closed-source project and 
none of it's code has been copied, only it's functionality.

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

#include "g_local.h"

// big ugly global buffer for use with buffered printing of long outputs
static char g_bfb[ 32000 ];

// note: list ordered alphabetically
g_admin_cmd_t g_admin_cmds[ ] = 
  {
    {"adjustban", G_admin_adjustban, "adjustban",
      "change the length or reason of a ban.  time is specified as numbers "
      "followed by units 'w' (weeks), 'd' (days), 'h' (hours) or 'm' (minutes),"
      " or seconds if no units are specified",
      "[^5ban #^7] (^3length|reason^7)"
    },

    {"adminlog", G_admin_adminlog, "adminlog",
      "list recent admin activity",
      "(^5start id#|name|!command|-skip#^7) (^5search skip#^7)"
    },

    {"admintest", G_admin_admintest, "admintest",
      "display your current admin level",
      ""
    },

    {"allowbuild", G_admin_denybuild, "denybuild",
      "restore a player's ability to build",
      "[^3name|slot#^7]"
    },

    {"allowweapon", G_admin_denyweapon, "denyweapon",
      "restore a player's ability to use a weapon or class",
      "[^3name|slot#^7] [^3class|weapon|all^7]"
    },
    
    {"allready", G_admin_allready, "allready",
      "makes everyone ready in intermission",
      ""
    },
    
    {"ban", G_admin_ban, "ban",
      "ban a player by IP and GUID with an optional expiration time and reason."
      "  time is specified as numbers followed by units 'w' (weeks), 'd' "
      "(days), 'h' (hours) or 'm' (minutes), or seconds if no units are "
      "specified",
      "[^3name|slot#|IP^7] (^5time^7) (^5reason^7)"
    },
	
    {"buildlog", G_admin_buildlog, "buildlog",
      "display a list of recent builds and deconstructs, optionally specifying"
      " a team",
      "(^5xnum^7) (^5#skip^7) (^5-name|num^7) (^5a|h^7)",
    },
    
    {"cancelvote", G_admin_cancelvote, "cancelvote",
      "cancel a vote taking place",
      ""
    },
    
    {"cp", G_admin_cp, "cp",
      "display a message to all users",
      "[^3message^7]"
    },

    {"demo", G_admin_demo, "demo",
      "turn admin chat off for the caller so it does not appear in demos. "
      "this is a toggle use !demo again to turn warnings back on",
      ""
    },

    {"denybuild", G_admin_denybuild, "denybuild",
      "take away a player's ability to build",
      "[^3name|slot#^7]"
    },

    {"denyweapon", G_admin_denyweapon, "denyweapon",
      "take away a player's ability to use a weapon or class",
      "[^3name|slot#^7] [^3class|weapon^7]"
    },

    {"designate", G_admin_designate, "designate",
      "give the player designated builder privileges",
      "[^3name|slot#^7]"
    },
    
    {"devmap", G_admin_devmap, "devmap",
      "load a map with cheats (and optionally force layout)",
      "[^3mapname^7] (^5layout^7)"
    },
    
    {"drop", G_admin_drop, "drop",
      "kick a client from the server without log",
      "[^3name|slot#^7] [^3message^7]"
    },

    {"flag", G_admin_flag, "flag",
      "add an admin flag to a player, prefix flag with '-' to disallow the flag. "
      "console can use this command on admin levels by prefacing a '*' to the admin level value.",
      "[^3name|slot#|admin#|*adminlevel^7] (^5+^7|^5-^7)[^3flag^7]"
    },

    {"flaglist", G_admin_flaglist, "flag",
      "list all flags understood by this server",
      ""
    },

    {"forcespec", G_admin_forcespec, "forcespec",
      "disable joining of teams for a player",
      "[^3name|slot#^7]"
    },
    
    {"help", G_admin_help, "help",
      "display commands available to you or help on a specific command",
      "(^5command^7)"
    },
	
    {"info", G_admin_info, "info",
      "read the server's information files",
      "(^5subject^7)"
    },

    {"invisible", G_admin_invisible, "invisible",
      "hides a player so they cannot be seen in playerlists",
      ""
    },

    {"kick", G_admin_kick, "kick",
      "kick a player with an optional reason",
      "(^5reason^7)"
    },
	
    {"L0", G_admin_L0, "l0",
      "Sets a level 1 to level 0",
      "[^3name|slot#^7]"
    },
    
    {"L1", G_admin_L1, "l1",
      "Sets a level 0 to level 1",
      "[^3name|slot#^7]"
    },
    
    {"layoutsave", G_admin_layoutsave, "layoutsave",
      "save a map layout",
      "[^3mapname^7]"
    },
    
    {"listadmins", G_admin_listadmins, "listadmins",
      "display a list of all server admins and their levels",
      "(^5name|start admin#^7) (^5minimum level to display^7)"
    },
    
    {"listlayouts", G_admin_listlayouts, "listlayouts",
      "display a list of all available layouts for a map",
      "(^5mapname^7)"
    },
	
    {"listmaps", G_admin_listmaps, "listmaps",
      "display a list of available maps on the server",
      "(^5map name^7)"
    },

    {"listplayers", G_admin_listplayers, "listplayers",
      "display a list of players, their client numbers and their levels",
      ""
    },
    
    {"lock", G_admin_lock, "lock",
      "lock a team to prevent anyone from joining it",
      "[^3a|h^7]"
    },

    {"lockname", G_admin_lockname, "lockname",
     "lock/unlock a players name from changing/updating.",
     "[^3name|slot#^7]"
    },
    
    {"map", G_admin_map, "map",
      "load a map (and optionally force layout)",
      "[^3mapname^7] (^5layout^7)"
    },
	
    {"maplog", G_admin_maplog, "maplog",
      "show recently played maps",
      ""
    },

    {"mute", G_admin_mute, "mute",
      "mute a player, to prevent them from speaking, calling votes, using private messages, or renaming",
      "[^3name|slot#^7]"
    },
    
    {"namelog", G_admin_namelog, "namelog",
      "display a list of names used by recently connected players",
      "(^5name^7)"
    },

    {"nextmap", G_admin_nextmap, "nextmap",
      "go to the next map in the cycle",
      ""
    },

    {"nobuild", G_admin_nobuild, "nobuild",
     "make an area unbuildable",
     "[^3game units area^7] [ ^3game units height^7]"
    },

    {"nobuildsave", G_admin_nobuildsave, "nobuild",
     "save a map's nobuild markers",
     ""
    },

    {"passvote", G_admin_passvote, "passvote",
      "pass a vote currently taking place",
      ""
    },
	
    {"pause", G_admin_pause, "pause",
      "prevent a player from interacting with the game."
      "  * will pause all players, using no argument will pause game clock",
      "(^5name|slot|*^7)"
    },

    {"practice", G_admin_practice, "practice",
      "set practice mode for player names with 'clan tag', "
      "these players will be allowed to join any team regardless of balance. "
      "'map count' is number of maps to maintain practice mode",
      "[^3clan tag|off^7] [^3map count^7])"
    },

    {"putteam", G_admin_putteam, "putteam",
      "move players to a specified team",
      "[^3name|slot#^7] [^3h|a|s^7]"
    },

    {"readconfig", G_admin_readconfig, "readconfig",
      "reloads the admin config file and refreshes permission flags",
      ""
    },
    
    {"register", G_admin_register, "register",
      "Registers your name to protect it from being used by others or updates your admin name to your current name.",
      "[^3level^7] [^3password^7]"
    },

    {"rename", G_admin_rename, "rename",
      "rename a player",
      "[^3name|slot#^7] [^3new name^7]"
    },

    {"restart", G_admin_restart, "restart",
      "restart the current map (optionally using named layout or keeping/switching teams)",
      "(^5layout^7) (^5keepteams|switchteams|keepteamslock|switchteamslock^7)"
    },
	
    {"revert", G_admin_revert, "revert",
      "revert one or more buildlog events, optionally of only one team",
      "(^5xnum^7) (^5#ID^7) (^5-name|num^7) (^5a|h^7)"
    },

    {"rotation", G_admin_listrotation, "rotation",
       "display a list of maps that are in the active map rotation",
       ""
    },

    {"scrimm", G_admin_scrimm, "scrimm",
     "Turn scrimm mode ^2on^7/^1off",
     "^7[^3on|off^7]"
    },

    {"seen", G_admin_seen, "seen",
      "find the last time a player was on the server",
      "[^3name|admin#^7]"
    },

    {"setlevel", G_admin_setlevel, "setlevel",
      "sets the admin level of a player",
      "[^3name|slot#|admin#^7] [^3level^7]"
    },

    {"showbans", G_admin_showbans, "ban",
      "display a (partial) list of active bans.",
      "(^5start at ban#^7|name|IP)"
    },

    {"spec999", G_admin_spec999, "spec999",
      "move 999 pingers to the spectator team",
      ""},
      
    {"specme", G_admin_putmespec, "specme",
        "moves you to the spectators (can be done silently with the 's' argument)",
	"(^5s^7)"
    },

    {"stage", G_admin_stage, "stage",
     "Sets a team to a stage",
     "[^3a|h^7] [^51|2|3^7]"
    },
	
    {"suspendban", G_admin_suspendban, "suspendban",
      "suspend a ban for a length of time. time is specified as numbers "
      "followed by units 'w' (weeks), 'd' (days), 'h' (hours) or 'm' (minutes),"
      " or seconds if no units are specified",
      "[^5ban #^7] [^5length^7]"
    },

    {"time", G_admin_time, "time",
      "show the current local server time",
      ""},

    {"tklog", G_admin_tklog, "tklog",
      "list recent teamkill activity",
      "(^5start id#|name|-skip#^7) (^5search skip#^7)"
    },

    {"unban", G_admin_unban, "ban",
      "unbans a player specified by the slot as seen in showbans",
      "[^3ban #^7]"
    },
    
    {"unlock", G_admin_unlock, "lock",
      "unlock a locked team",
      "[^3a|h^7]"
    },

    {"undesignate", G_admin_designate, "designate",
      "revoke designated builder privileges",
      "[^3name|slot#^7]"
    },

    {"unflag", G_admin_flag, "flag",
      "clears an admin flag from a player. "
      "console can use this command on admin levels by prefacing a '*' to the admin level value.",
      "[^3name|slot#|admin#|*adminlevel^7] (^5+^7|^5-^7)[^3flag^7]"
    },
	
    {"unforcespec", G_admin_unforcespec, "forcespec",
      "enable joining of teams for a player",
      "[^3name|slot#^7]"
     },
    
    {"unmute", G_admin_mute, "mute",
      "unmute a muted player",
      "[^3name|slot#^7]"
    },
	
    {"unpause", G_admin_pause, "pause",
      "allow a player to interact with the game."
      "  * will unpause all players, using no argument will unpause game clock",
      "(^5name|slot|*^7)"
     }, 

    {
     "warn", G_admin_warn, "warn",
      "Warn a player to cease or face admin intervention",
      "[^3name|slot#^7] [reason]"
    },

  };

static int adminNumCmds = sizeof( g_admin_cmds ) / sizeof( g_admin_cmds[ 0 ] );

static int admin_level_maxname = 0;
g_admin_level_t *g_admin_levels[ MAX_ADMIN_LEVELS ];
g_admin_admin_t *g_admin_admins[ MAX_ADMIN_ADMINS ];
g_admin_ban_t *g_admin_bans[ MAX_ADMIN_BANS ];
g_admin_command_t *g_admin_commands[ MAX_ADMIN_COMMANDS ];
g_admin_namelog_t *g_admin_namelog[ MAX_ADMIN_NAMELOGS ];

static int admin_adminlog_index = 0;
g_admin_adminlog_t *g_admin_adminlog[ MAX_ADMIN_ADMINLOGS ];

static int admin_tklog_index = 0;
g_admin_tklog_t *g_admin_tklog[ MAX_ADMIN_TKLOGS ];

// match a certain flag within these flags
// return state of whether flag was found or not,
// set *perm to indicate whether found flag was + or -
static qboolean admin_permission( char *flags, const char *flag, qboolean *perm )
{
  char *token, *token_p = flags;
  while( *( token = COM_Parse( &token_p ) ) )
  {
    *perm = qtrue;
    if( *token == '-' || *token == '+' )
      *perm = *token++ == '+';
    if( !strcmp( token, flag ) )
      return qtrue;
    if( !strcmp( token, ADMF_ALLFLAGS ) )
    {
      while( *( token = COM_Parse( &token_p ) ) )
      {
        // -ALLFLAGS +flag or ALLFLAGS -flag
        if( *token != ( *perm ? '-' : '+' ) )
          continue;
        if( !strcmp( token + 1, flag ) )
        {
          *perm = !*perm;
          break;
        }
      }
      return qtrue;
    }
  }
  return qfalse;
}

// This function should only be used directly when the client is connecting and thus has no GUID.
// Else, use G_admin_permission()
qboolean G_admin_permission_guid( char *guid, const char* flag )
{
  int i;
  int l = 0;
  qboolean perm = qfalse;

  // Does the admin specifically have this flag granted/denied to them,
  // irrespective of their admin level?
  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    if( !Q_stricmp( guid, g_admin_admins[ i ]->guid ) )
    {
      if( admin_permission( g_admin_admins[ i ]->flags, flag, &perm ) )
        return perm;
      l = g_admin_admins[ i ]->level;
      break;
    }
  }

  // If not, is this flag granted/denied for their admin level?
  for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
  {
    if( g_admin_levels[ i ]->level == l )
      return admin_permission( g_admin_levels[ i ]->flags, flag, &perm ) &&
        perm;
  }
  return qfalse;
}

qboolean G_admin_permission( gentity_t *ent, const char *flag )
{
  if(!ent) return qtrue; //console always wins

  return G_admin_permission_guid(ent->client->pers.guid, flag);
}

qboolean G_admin_name_check( gentity_t *ent, char *name, char *err, int len )
{
  int i;
  gclient_t *client;
  char testName[ MAX_NAME_LENGTH ] = {""};
  char name2[ MAX_NAME_LENGTH ] = {""};
  int alphaCount = 0;

  G_SanitiseString( name, name2, sizeof( name2 ) );

  if( !Q_stricmp( name2, "UnnamedPlayer" ) ) 
    return qtrue;

  if( !Q_stricmp( name2, "console" ) )
  {
    Q_strncpyz( err, va( "The name '%s^7' is invalid here", name2 ),
      len );
    return qfalse;
  }

  for( i = 0; i < level.maxclients; i++ )
  {
    client = &level.clients[ i ];
    if( client->pers.connected != CON_CONNECTING
      && client->pers.connected != CON_CONNECTED ) 
    {
      continue;
    }

    // can rename ones self to the same name using different colors
    if( i == ( ent - g_entities ) )
      continue;

    G_SanitiseString( client->pers.netname, testName, sizeof( testName ) );
    if( !Q_stricmp( name2, testName ) )
    {
      Q_strncpyz( err, va( "The name '%s^7' is already in use", name ),
        len );
      return qfalse;
    }
  }
   
  if( '0' <= name2[ 0 ]  && name2[ 0 ] <= '9' )
  {
    Q_strncpyz( err, "Names cannot begin with a number. Please choose another.", len );
    return qfalse;
  }
  
  for( i = 0; name2[ i ] !='\0'; i++)
  {
    if( Q_isalpha( name2[ i ] ) )
     alphaCount++;
    if( name2[ i ] == '/' )
    {
      if( name2[ i + 1 ] == '/' || name2[ i + 1 ] == '*' )
      {
        Q_strncpyz( err, "Names cannot contain '//' or '/*'. Please choose another.", len );
        return qfalse;
      }
    }
  }
  
  if( alphaCount == 0 ) 
  {
    Q_strncpyz( err, va( "The name '%s^7' does not include at least one letter. Please choose another.", name ), len );
    return qfalse;
  }

  if( !g_adminNameProtect.string[ 0 ] )
    return qtrue;

  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    if( g_admin_admins[ i ]->level < 1 )
      continue;
    G_SanitiseString( g_admin_admins[ i ]->name, testName, sizeof( testName ) );
    if( !Q_stricmp( name2, testName ) &&
      Q_stricmp( ent->client->pers.guid, g_admin_admins[ i ]->guid ) )
    {
      Q_strncpyz( err, va( "The name '%s^7' belongs to an admin. "
        "Please choose another.", name ), len );
      return qfalse;
    }
  }
  return qtrue;
}

static qboolean admin_higher_guid( char *admin_guid, char *victim_guid )
{
  int i;
  int alevel = 0;

  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    if( !Q_stricmp( admin_guid, g_admin_admins[ i ]->guid ) )
    {
      alevel = g_admin_admins[ i ]->level;
      break;
    }
  }
  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    if( !Q_stricmp( victim_guid, g_admin_admins[ i ]->guid ) )
    {
      if( alevel < g_admin_admins[ i ]->level )
        return qfalse;
      if( strstr( g_admin_admins[ i ]->flags, va( "%s", ADMF_IMMUTABLE ) ) )
        return qfalse;
    }
  }
  return qtrue;
}

static qboolean admin_higher( gentity_t *admin, gentity_t *victim )
{

  // console always wins
  if( !admin )
    return qtrue;
  // just in case
  if( !victim )
    return qtrue;

  return admin_higher_guid( admin->client->pers.guid,
    victim->client->pers.guid );
}

void admin_writeconfig_string( char *s, fileHandle_t f )
{
  char buf[ MAX_STRING_CHARS ];

  buf[ 0 ] = '\0';
  if( s[ 0 ] )
  {
    //Q_strcat(buf, sizeof(buf), s);
    Q_strncpyz( buf, s, sizeof( buf ) );
    trap_FS_Write( buf, strlen( buf ), f );
  }
  trap_FS_Write( "\n", 1, f );
}

void admin_writeconfig_int( int v, fileHandle_t f )
{
  char buf[ 32 ];

  Com_sprintf( buf, sizeof(buf), "%d", v );
  if( buf[ 0 ] )
    trap_FS_Write( buf, strlen( buf ), f );
  trap_FS_Write( "\n", 1, f );
}

void G_admin_writeconfig( void )
{
  fileHandle_t f;
  int len, i;
  qtime_t qt;
  int t;

  if( !g_admin.string[ 0 ] )
  {
    G_Printf( S_COLOR_YELLOW "WARNING: g_admin is not set. "
      " configuration will not be saved to a file.\n" );
    return;
  }
  t = trap_RealTime( &qt );
  len = trap_FS_FOpenFile( g_admin.string, &f, FS_WRITE );
  if( len < 0 )
  {
    G_Printf( "G_admin_writeconfig: could not open g_admin file \"%s\"\n",
              g_admin.string );
    return;
  }
  for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
  {
    trap_FS_Write( "[level]\n", 8, f );
    trap_FS_Write( "level   = ", 10, f );
    admin_writeconfig_int( g_admin_levels[ i ]->level, f );
    trap_FS_Write( "name    = ", 10, f );
    admin_writeconfig_string( g_admin_levels[ i ]->name, f );
    trap_FS_Write( "flags   = ", 10, f );
    admin_writeconfig_string( g_admin_levels[ i ]->flags, f );
    trap_FS_Write( "\n", 1, f );
  }
  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    // don't write level 0 users
    if( g_admin_admins[ i ]->level < 1 )
      continue;

    trap_FS_Write( "[admin]\n", 8, f );
    trap_FS_Write( "name    = ", 10, f );
    admin_writeconfig_string( g_admin_admins[ i ]->name, f );
    trap_FS_Write( "guid    = ", 10, f );
    admin_writeconfig_string( g_admin_admins[ i ]->guid, f );
    trap_FS_Write( "level   = ", 10, f );
    admin_writeconfig_int( g_admin_admins[ i ]->level, f );
    trap_FS_Write( "flags   = ", 10, f );
    admin_writeconfig_string( g_admin_admins[ i ]->flags, f );
    trap_FS_Write( "seen    = ", 10, f );
    admin_writeconfig_int( g_admin_admins[ i ]->seen, f );
    trap_FS_Write( "\n", 1, f );
  }
  for( i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++ )
  {
    // don't write expired bans
    // if expires is 0, then it's a perm ban
    if( g_admin_bans[ i ]->expires != 0 &&
         ( g_admin_bans[ i ]->expires - t ) < 1 )
      continue;

    trap_FS_Write( "[ban]\n", 6, f );
    trap_FS_Write( "name    = ", 10, f );
    admin_writeconfig_string( g_admin_bans[ i ]->name, f );
    trap_FS_Write( "guid    = ", 10, f );
    admin_writeconfig_string( g_admin_bans[ i ]->guid, f );
    trap_FS_Write( "ip      = ", 10, f );
    admin_writeconfig_string( g_admin_bans[ i ]->ip, f );
    trap_FS_Write( "reason  = ", 10, f );
    admin_writeconfig_string( g_admin_bans[ i ]->reason, f );
    trap_FS_Write( "made    = ", 10, f );
    admin_writeconfig_string( g_admin_bans[ i ]->made, f );
    trap_FS_Write( "expires = ", 10, f );
    admin_writeconfig_int( g_admin_bans[ i ]->expires, f );
    if( g_admin_bans[ i ]->suspend > t )
    {
      trap_FS_Write( "suspend = ", 10, f );
      admin_writeconfig_int( g_admin_bans[ i ]->suspend, f );
    }
    trap_FS_Write( "banner  = ", 10, f );
    admin_writeconfig_string( g_admin_bans[ i ]->banner, f );
    trap_FS_Write( "\n", 1, f );
  }
  for( i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++ )
  {
    trap_FS_Write( "[command]\n", 10, f );
    trap_FS_Write( "command = ", 10, f );
    admin_writeconfig_string( g_admin_commands[ i ]->command, f );
    trap_FS_Write( "exec    = ", 10, f );
    admin_writeconfig_string( g_admin_commands[ i ]->exec, f );
    trap_FS_Write( "desc    = ", 10, f );
    admin_writeconfig_string( g_admin_commands[ i ]->desc, f );
    trap_FS_Write( "flag    = ", 10, f );
    admin_writeconfig_string( g_admin_commands[ i ]->flag, f );

    trap_FS_Write( "\n", 1, f );
  }
  trap_FS_FCloseFile( f );
}

static void admin_readconfig_string( char **cnf, char *s, int size )
{
  char * t;

  //COM_MatchToken(cnf, "=");
  t = COM_ParseExt( cnf, qfalse );
  if( !strcmp( t, "=" ) )
  {
    t = COM_ParseExt( cnf, qfalse );
  }
  else
  {
    G_Printf( "readconfig: warning missing = before "
              "\"%s\" on line %d\n",
              t,
              COM_GetCurrentParseLine() );
  }
  s[ 0 ] = '\0';
  while( t[ 0 ] )
  {
    if( ( s[ 0 ] == '\0' && strlen( t ) <= size ) 
      || ( strlen( t ) + strlen( s ) < size ) )
    {

      Q_strcat( s, size, t );
      Q_strcat( s, size, " " );
    }
    t = COM_ParseExt( cnf, qfalse );
  }
  // trim the trailing space
  if( strlen( s ) > 0 && s[ strlen( s ) - 1 ] == ' ' )
    s[ strlen( s ) - 1 ] = '\0';
}

static void admin_readconfig_int( char **cnf, int *v )
{
  char * t;

  //COM_MatchToken(cnf, "=");
  t = COM_ParseExt( cnf, qfalse );
  if( !strcmp( t, "=" ) )
  {
    t = COM_ParseExt( cnf, qfalse );
  }
  else
  {
    G_Printf( "readconfig: warning missing = before "
              "\"%s\" on line %d\n",
              t,
              COM_GetCurrentParseLine() );
  }
  *v = atoi( t );
}

// if we can't parse any levels from readconfig, set up default
// ones to make new installs easier for admins
static void admin_default_levels( void )
{
  g_admin_level_t * l;
  int i;

  for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
  {
    G_Free( g_admin_levels[ i ] );
    g_admin_levels[ i ] = NULL;
  }
  for( i = 0; i <= 5; i++ )
  {
    l = G_Alloc( sizeof( g_admin_level_t ) );
    l->level = i;
    *l->name = '\0';
    *l->flags = '\0';
    g_admin_levels[ i ] = l;
  }
  Q_strncpyz( g_admin_levels[ 0 ]->name, "^4Unknown Player",
    sizeof( l->name ) );
  Q_strncpyz( g_admin_levels[ 0 ]->flags, "listmaps listplayers admintest help specme time", sizeof( l->flags ) );

  Q_strncpyz( g_admin_levels[ 1 ]->name, "^5Server Regular",
    sizeof( l->name ) );
  Q_strncpyz( g_admin_levels[ 1 ]->flags, "listmaps listplayers admintest help specme time", sizeof( l->flags ) );

  Q_strncpyz( g_admin_levels[ 2 ]->name, "^6Team Manager",
    sizeof( l->name ) );
  Q_strncpyz( g_admin_levels[ 2 ]->flags, "listmaps listplayers admintest help specme time"
                                          " putteam spec999 warn denybuild denyweapon", sizeof( l->flags ) );

  Q_strncpyz( g_admin_levels[ 3 ]->name, "^2Junior Admin",
    sizeof( l->name ) );
  Q_strncpyz( g_admin_levels[ 3 ]->flags, "listmaps listplayers admintest help specme time"
                                          " putteam spec999 warn denybuild denyweapon"
                                          " lock kick mute SEESFULLLISTPLAYERS ADMINCHAT", sizeof( l->flags ) );

  Q_strncpyz( g_admin_levels[ 4 ]->name, "^3Senior Admin",
    sizeof( l->name ) );
  Q_strncpyz( g_admin_levels[ 4 ]->flags, "listmaps listplayers admintest help specme time"
                                          " putteam spec999 warn denybuild denyweapon"
                                          " lock kick mute SEESFULLLISTPLAYERS ADMINCHAT"
                                          " ban showbans suspendban", sizeof( l->flags ) );

  Q_strncpyz( g_admin_levels[ 5 ]->name, "^1Server Operator",
    sizeof( l->name ) );
  Q_strncpyz( g_admin_levels[ 5 ]->flags, "ALLFLAGS -INCOGNITO -IMMUTABLE -DBUILDER -BANIMMUNITY", sizeof( l->flags ) );
}

//  return a level for a player entity.
int G_admin_level( gentity_t *ent )
{
  int i;
  qboolean found = qfalse;

  if( !ent )
  {
    return MAX_ADMIN_LEVELS;
  }

  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    if( !Q_stricmp( g_admin_admins[ i ]->guid, ent->client->pers.guid ) )
    {

      found = qtrue;
      break;
    }
  }

  if( found )
  {
    return g_admin_admins[ i ]->level;
  }

  return 0;
}

static void admin_log( gentity_t *admin, char *cmd, int skiparg )
{
  fileHandle_t f;
  int len, i, j;
  char string[ MAX_STRING_CHARS ], decolored[ MAX_STRING_CHARS ];
  int min, tens, sec;
  g_admin_admin_t *a;
  g_admin_level_t *l;
  char flags[ MAX_ADMIN_FLAGS * 2 ];
  gentity_t *victim = NULL;
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ];

  if( !g_adminLog.string[ 0 ] )
    return ;


  len = trap_FS_FOpenFile( g_adminLog.string, &f, FS_APPEND );
  if( len < 0 )
  {
    G_Printf( "admin_log: error could not open %s\n", g_adminLog.string );
    return ;
  }

  sec = (level.time - level.startTime) / 1000;
  min = sec / 60;
  sec -= min * 60;
  tens = sec / 10;
  sec -= tens * 10;

  *flags = '\0';
  if( admin )
  {
    for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
    {
      if( !Q_stricmp( g_admin_admins[ i ]->guid , admin->client->pers.guid ) )
      {

        a = g_admin_admins[ i ];
        Q_strncpyz( flags, a->flags, sizeof( flags ) );
        for( j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++ )
        {
          if( g_admin_levels[ j ]->level == a->level )
          {
            l = g_admin_levels[ j ];
            Q_strcat( flags, sizeof( flags ), l->flags );
            break;
          }
        }
        break;
      }
    }
  }

  if( G_SayArgc() > 1 + skiparg )
  {
    G_SayArgv( 1 + skiparg, name, sizeof( name ) );
    if( G_ClientNumbersFromString( name, pids ) == 1 )
    {
      victim = &g_entities[ pids[ 0 ] ];
    }
  }

  if( victim && Q_stricmp( cmd, "attempted" ) )
  {
    Com_sprintf( string, sizeof( string ),
                 "%3i:%i%i: %i: %s: %s: %s: %s: %s: %s: \"%s\"\n",
                 min,
                 tens,
                 sec,
                 ( admin ) ? admin->s.clientNum : -1,
                 ( admin ) ? admin->client->pers.guid
                 : "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                 ( admin ) ? admin->client->pers.netname : "console",
                 flags,
                 cmd,
                 victim->client->pers.guid,
                 victim->client->pers.netname,
                 G_SayConcatArgs( 2 + skiparg ) );
  }
  else
  {
    Com_sprintf( string, sizeof( string ),
                 "%3i:%i%i: %i: %s: %s: %s: %s: \"%s\"\n",
                 min,
                 tens,
                 sec,
                 ( admin ) ? admin->s.clientNum : -1,
                 ( admin ) ? admin->client->pers.guid
                 : "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX",
                 ( admin ) ? admin->client->pers.netname : "console",
                 flags,
                 cmd,
                 G_SayConcatArgs( 1 + skiparg ) );
  }

  if( g_decolorLogfiles.integer )
  {
    G_DecolorString( string, decolored );
    trap_FS_Write( decolored, strlen( decolored ), f );
  }
  else
  {
     trap_FS_Write( string, strlen( string ), f );
  }
  
  trap_FS_FCloseFile( f );
  
  if ( !Q_stricmp( cmd, "attempted" ) )
  {
    Com_sprintf( string, sizeof( string ),
                 "%s^7 (%i) %s: %s",
                 ( admin ) ? admin->client->pers.netname : "console",
                 ( admin ) ? admin->s.clientNum : -1,
                 cmd,
                 G_SayConcatArgs( 1 + skiparg ) );
    G_AdminsPrintf("%s^7\n",string);
  }
}

static int admin_listadmins( gentity_t *ent, int start, char *search, int minlevel )
{
  int drawn = 0;
  char guid_stub[9];
  char name[ MAX_NAME_LENGTH ] = {""};
  char name2[ MAX_NAME_LENGTH ] = {""};
  char lname[ MAX_NAME_LENGTH ] = {""};
  char lname_fmt[ 5 ];
  int i,j;
  gentity_t *vic;
  int l = 0;
  qboolean dup = qfalse;
  
  ADMBP_begin();

  // print out all connected players regardless of level if name searching
  for( i = 0; i < level.maxclients && search[ 0 ]; i++ )
  {
    vic = &g_entities[ i ];

    if( vic->client && vic->client->pers.connected != CON_CONNECTED )
      continue;
    
    l = vic->client->pers.adminLevel;

    G_SanitiseString( vic->client->pers.netname, name, sizeof( name ) );
    if( !strstr( name, search ) )
      continue;

    for( j = 0; j < 8; j++ )
      guid_stub[ j ] = vic->client->pers.guid[ j + 24 ];
    guid_stub[ j ] = '\0';
  
    lname[ 0 ] = '\0'; 
    Q_strncpyz( lname_fmt, "%s", sizeof( lname_fmt ) );
    for( j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++ )
    {
      if( g_admin_levels[ j ]->level == l )
      {
        G_DecolorString( g_admin_levels[ j ]->name, lname );
        Com_sprintf( lname_fmt, sizeof( lname_fmt ), "%%%is",
          (int)( admin_level_maxname + strlen( g_admin_levels[ j ]->name )
            - strlen( lname ) ) );
        Com_sprintf( lname, sizeof( lname ), lname_fmt,
           g_admin_levels[ j ]->name );
        break;
      }
    }
    ADMBP( va( "%4i %4i %s^7 (*%s) ^1%1s ^7%s^7\n",
      i,
      l,
      lname,
      guid_stub,
      ( G_admin_permission( &g_entities[ i ], ADMF_BAN_IMMUNITY ) ) ? "I" : "",
      vic->client->pers.netname ) );
    drawn++;
  }

  for( i = start; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]
    && drawn < MAX_ADMIN_LISTITEMS; i++ )
   if( g_admin_admins[ i ]->level >= minlevel ) 
   {
     if( search[ 0 ] )
     {
       G_SanitiseString( g_admin_admins[ i ]->name, name, sizeof( name ) );
       if( !strstr( name, search ) )
         continue;
      
       // verify we don't have the same guid/name pair in connected players
       // since we don't want to draw the same player twice
       dup = qfalse;
       for( j = 0; j < level.maxclients; j++ )
       {
         vic = &g_entities[ j ];
         if( !vic->client || vic->client->pers.connected != CON_CONNECTED )
           continue;
         G_SanitiseString( vic->client->pers.netname, name2, sizeof( name2 ) );
         if( !Q_stricmp( vic->client->pers.guid, g_admin_admins[ i ]->guid )
           && strstr( name2, search ) ) 
         {
           dup = qtrue;
           break;
         }
       }
       if( dup )
         continue;
     }
     for( j = 0; j < 8; j++ )
       guid_stub[ j ] = g_admin_admins[ i ]->guid[ j + 24 ];
     guid_stub[ j ] = '\0';
    
     lname[ 0 ] = '\0'; 
     Q_strncpyz( lname_fmt, "%s", sizeof( lname_fmt ) );
     for( j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++ )
     {
       if( g_admin_levels[ j ]->level == g_admin_admins[ i ]->level )
       {
         G_DecolorString( g_admin_levels[ j ]->name, lname );
         Com_sprintf( lname_fmt, sizeof( lname_fmt ), "%%%is",
           (int)( admin_level_maxname + strlen( g_admin_levels[ j ]->name )
             - strlen( lname ) ) );
         Com_sprintf( lname, sizeof( lname ), lname_fmt,
            g_admin_levels[ j ]->name );
         break;
       }
     }
     ADMBP( va( "%4i %4i %s^7 (*%s) ^1%1s ^7%s^7\n",
       ( i + MAX_CLIENTS ),
       g_admin_admins[ i ]->level,
       lname,
       guid_stub,
       G_admin_permission_guid( g_admin_admins[ i ]->guid, ADMF_BAN_IMMUNITY ) ? "I" : "",
       g_admin_admins[ i ]->name ) );
     drawn++;
   }
  ADMBP_end();
  return drawn;
}

void G_admin_duration( int secs, char *duration, int dursize )
{

  if( secs > ( 60 * 60 * 24 * 365 * 50 ) || secs < 0 )
    Q_strncpyz( duration, "PERMANENT", dursize );
  else if( secs >= ( 60 * 60 * 24 * 365 ) )
    Com_sprintf( duration, dursize, "%1.1f years",
      ( secs / ( 60 * 60 * 24 * 365.0f ) ) );
  else if( secs >= ( 60 * 60 * 24 * 90 ) )
    Com_sprintf( duration, dursize, "%1.1f weeks",
      ( secs / ( 60 * 60 * 24 * 7.0f ) ) );
  else if( secs >= ( 60 * 60 * 24 ) )
    Com_sprintf( duration, dursize, "%1.1f days",
      ( secs / ( 60 * 60 * 24.0f ) ) );
  else if( secs >= ( 60 * 60 ) )
    Com_sprintf( duration, dursize, "%1.1f hours",
      ( secs / ( 60 * 60.0f ) ) );
  else if( secs >= 60 )
    Com_sprintf( duration, dursize, "%1.1f minutes",
      ( secs / 60.0f ) );
  else
    Com_sprintf( duration, dursize, "%i seconds", secs );
}

qboolean G_admin_ban_check( char *userinfo, char *reason, int rlen )
{
  static char lastConnectIP[ 16 ] = {""};
  static int lastConnectTime = 0;
  char guid[ 33 ];
  char ip[ 16 ];
  char *value;
  qtime_t qt;
  int t;
  int i;
 
  *reason = '\0'; 
  if( !*userinfo )
    return qfalse;

  value = Info_ValueForKey( userinfo, "ip" );
  Q_strncpyz( ip, value, sizeof( ip ) );
  // strip port
  value = strchr( ip, ':' );
  if ( value )
    *value = '\0';
  if( !*ip )
    return qfalse;

  value = Info_ValueForKey( userinfo, "cl_guid" );
  Q_strncpyz( guid, value, sizeof( guid ) );

  t  = trap_RealTime( &qt ); 
  for( i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++ )
  {
    // 0 is for perm ban
    if( g_admin_bans[ i ]->expires != 0 &&
      ( g_admin_bans[ i ]->expires - t ) < 1 )
      continue;
    if( g_admin_bans[ i ]->suspend >= t )
      continue;
    if( strstr( ip, g_admin_bans[ i ]->ip ) == ip )
    {
      char duration[ 32 ];
      qboolean immune = qfalse;
      
      // ban immunity
      if( *guid )
      {
        int j;

        for( j = 0; j < MAX_ADMIN_ADMINS && g_admin_admins[ j ]; j++ )
        {
          if( !Q_stricmp( guid, g_admin_admins[ j ]->guid ) &&
            G_admin_permission_guid( guid, ADMF_BAN_IMMUNITY ) )
          {
            immune = qtrue;
            break;
          }
        }
      }
      if( !immune )
      {
        G_admin_duration( ( g_admin_bans[ i ]->expires - t ),
          duration, sizeof( duration ) );

        if( t - lastConnectTime >= 10 ||
          Q_stricmp( lastConnectIP, ip ) != 0 )
        {
          lastConnectTime = t;
          Q_strncpyz( lastConnectIP, ip, sizeof( lastConnectIP ) );

          G_AdminsPrintf(
            "Banned player %s^7 (%s^7) tried to connect (ban #%i by %s^7 expires %s reason: %s^7 )\n",
            Info_ValueForKey( userinfo, "name" ),
            g_admin_bans[ i ]->name,
            i+1,
            g_admin_bans[ i ]->banner,
            duration,
            g_admin_bans[ i ]->reason );
        }

        Com_sprintf(
          reason,
          rlen,
          "You have been banned by %s^7 reason: %s^7 expires: %s",
          g_admin_bans[ i ]->banner,
          g_admin_bans[ i ]->reason,
          duration );
        G_LogPrintf("Banned player tried to connect from IP %s\n", ip);

        return qtrue;
      }
    }
    if( *guid && !Q_stricmp( g_admin_bans[ i ]->guid, guid ) )
    {
      char duration[ 32 ];
      G_admin_duration( ( g_admin_bans[ i ]->expires - t ),
        duration, sizeof( duration ) );
      Com_sprintf(
        reason,
        rlen,
        "You have been banned by %s^7 reason: %s^7 expires: %s",
        g_admin_bans[ i ]->banner,
        g_admin_bans[ i ]->reason,
        duration
      );
      G_Printf("Banned player tried to connect with GUID %s\n", guid);
      return qtrue;
    }
  }
  if ( *guid )
  {
    int count = 0;
    qboolean valid = qtrue;

    while( guid[ count ] != '\0' && valid )
    {
      if( (guid[ count ] < '0' || guid[ count ] > '9') &&
          (guid[ count ] < 'A' || guid[ count ] > 'F') )
      {
        valid = qfalse;
      }
      count++;
    }
    if( !valid || count != 32 )
    {
      Com_sprintf( reason, rlen, "Invalid client data" );
      G_Printf("Player with invalid GUID [%s] connect from IP %s\n", guid, ip);
      return qtrue;
    }
  }
  return qfalse;
}

qboolean G_admin_cmd_check( gentity_t *ent, qboolean say )
{
  int i;
  char command[ MAX_ADMIN_CMD_LEN ];
  char *cmd;
  int skip = 0;

  command[ 0 ] = '\0';
  G_SayArgv( 0, command, sizeof( command ) );
  if( !Q_stricmp( command, "say" ) ||
       ( G_admin_permission( ent, ADMF_TEAMCHAT_CMD ) &&
         ( !Q_stricmp( command, "say_team" ) ) ) )
  {
    skip = 1;
    G_SayArgv( 1, command, sizeof( command ) );
  }
  if( !command[ 0 ] )
    return qfalse;

  if( command[ 0 ] == '!' )
  {
    cmd = &command[ 1 ];
  }
  else
  {
    return qfalse;
  }
  
  // Flood limit.  If they're talking too fast, determine that and return.
  if( g_floodMinTime.integer )
   if ( G_Flood_Limited( ent ) )
   {
    trap_SendServerCommand( ent-g_entities, "print \"Your chat is flood-limited; wait before chatting again\n\"" );
    return qtrue;
   }

  for( i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++ )
  {
    if( Q_stricmp( cmd, g_admin_commands[ i ]->command ) )
      continue;

    if( G_admin_permission( ent, g_admin_commands[ i ]->flag ) )
    {
      int j;

      trap_Cvar_Register( NULL, "arg_all", "", CVAR_TEMP | CVAR_ROM | CVAR_USER_CREATED );
      trap_Cvar_Set( "arg_all", G_SayConcatArgs( skip + 1 ) );

      trap_Cvar_Register( NULL, "arg_count", "", CVAR_TEMP | CVAR_ROM | CVAR_USER_CREATED );
      trap_Cvar_Set( "arg_count", va( "%i", G_SayArgc() - ( skip + 1 ) ) );

      trap_Cvar_Register( NULL, "arg_client", "", CVAR_TEMP | CVAR_ROM | CVAR_USER_CREATED );
      trap_Cvar_Set( "arg_client", ( ent ) ? ent->client->pers.netname : "console" );

      for (j = G_SayArgc() - ( skip + 1 ); j; j--)
      {
        char this_arg[ MAX_CVAR_VALUE_STRING ];
        trap_Cvar_Register( NULL, va( "arg_%i", j ), "", CVAR_TEMP | CVAR_ROM | CVAR_USER_CREATED );
        G_SayArgv( j + skip, this_arg, sizeof( this_arg ) );
        trap_Cvar_Set( va( "arg_%i", j ), this_arg );
      }

      trap_SendConsoleCommand( EXEC_APPEND, g_admin_commands[ i ]->exec );
      admin_log( ent, cmd, skip );
      G_admin_adminlog_log( ent, cmd, NULL, skip, qtrue );
    }
    else
    {
      ADMP( va( "^3!%s: ^7permission denied\n", g_admin_commands[ i ]->command ) );
      admin_log( ent, "attempted", skip - 1 );
      G_admin_adminlog_log( ent, cmd, NULL, skip, qfalse );
    }
    return qtrue;
  }

  for( i = 0; i < adminNumCmds; i++ )
  {
    if( Q_stricmp( cmd, g_admin_cmds[ i ].keyword ) )
      continue;

    if( G_admin_permission( ent, g_admin_cmds[ i ].flag ) )
    {
      g_admin_cmds[ i ].handler( ent, skip );
      admin_log( ent, cmd, skip );
      G_admin_adminlog_log( ent, cmd, NULL, skip, qtrue );
    }
    else
    {
      ADMP( va( "^3!%s: ^7permission denied\n", g_admin_cmds[ i ].keyword ) );
      admin_log( ent, "attempted", skip - 1 );
      G_admin_adminlog_log( ent, cmd, NULL, skip, qfalse );
    }
    return qtrue;
  }
  return qfalse;
}

void G_admin_namelog_cleanup( )
{
  int i;

  for( i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++ )
  {
    G_Free( g_admin_namelog[ i ] );
    g_admin_namelog[ i ] = NULL;
  }
}

void G_admin_namelog_update( gclient_t *client, qboolean disconnect )
{
  int i, j;
  g_admin_namelog_t *namelog;
  char n1[ MAX_NAME_LENGTH ];
  char n2[ MAX_NAME_LENGTH ];
  char countryName[ MAX_COUNTRY_LENGTH ];
  
  int clientNum = ( client - level.clients );

  if( client->sess.invisible == qfalse )
    G_admin_seen_update( client->pers.guid );

  G_SanitiseString( client->pers.netname, n1, sizeof( n1 ) );
  for( i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++ )
  {
    if( disconnect && g_admin_namelog[ i ]->slot != clientNum )
      continue;

    if( !disconnect && !( g_admin_namelog[ i ]->slot == clientNum ||
                          g_admin_namelog[ i ]->slot == -1 ) )
    {
      continue;
    }

    if( !Q_stricmp( client->pers.ip, g_admin_namelog[ i ]->ip )
      && !Q_stricmp( client->pers.guid, g_admin_namelog[ i ]->guid ) )
    {
      for( j = 0; j < MAX_ADMIN_NAMELOG_NAMES
        && g_admin_namelog[ i ]->name[ j ][ 0 ]; j++ )
      {
        G_SanitiseString( g_admin_namelog[ i ]->name[ j ], n2, sizeof( n2 ) );
        if( !Q_stricmp( n1, n2 ) ) 
          break;
      }
      if( j == MAX_ADMIN_NAMELOG_NAMES )
        j = MAX_ADMIN_NAMELOG_NAMES - 1;
      Q_strncpyz( g_admin_namelog[ i ]->name[ j ], client->pers.netname,
        sizeof( g_admin_namelog[ i ]->name[ j ] ) );
      g_admin_namelog[ i ]->slot = ( disconnect ) ? -1 : clientNum;
 
      // if this player is connecting, they are no longer banned
      if( !disconnect )
        g_admin_namelog[ i ]->banned = qfalse;

      //check other things like if user was denybuild or muted or denyweapon and restore them
      if( !disconnect )
      {
        if( g_admin_namelog[ i ]->muted )
        {
            client->pers.muted = qtrue;
            //client->pers.muteExpires = g_admin_namelog[ i ]->muteExpires;
            G_AdminsPrintf( "^7%s^7's mute has been restored\n", client->pers.netname );
            g_admin_namelog[ i ]->muted = qfalse;
        }
        if( g_admin_namelog[ i ]->denyBuild )
        {
            client->pers.denyBuild = qtrue;
            G_AdminsPrintf( "^7%s^7's Denybuild has been restored\n", client->pers.netname );
            g_admin_namelog[ i ]->denyBuild = qfalse;
        }
        if( g_admin_namelog[ i ]->denyHumanWeapons > 0 || g_admin_namelog[ i ]->denyAlienClasses > 0 )
        {
            if( g_admin_namelog[ i ]->denyHumanWeapons > 0 )
                client->pers.denyHumanWeapons =  g_admin_namelog[ i ]->denyHumanWeapons;
            if( g_admin_namelog[ i ]->denyAlienClasses > 0 )
                client->pers.denyAlienClasses =  g_admin_namelog[ i ]->denyAlienClasses;

            G_AdminsPrintf( "^7%s^7's Denyweapon has been restored\n", client->pers.netname );
            g_admin_namelog[ i ]->denyHumanWeapons = 0;
            g_admin_namelog[ i ]->denyAlienClasses = 0;
        }
        if( g_admin_namelog[ i ]->forcespeced )
        {
            client->pers.specd = qtrue;
            G_AdminsPrintf( "^7%s^7's force specing has been restored\n", client->pers.netname );
            g_admin_namelog[ i ]->forcespeced = qfalse;
        }
        /*
        if( g_admin_namelog[ i ]->specExpires > 0 )
        {
            client->pers.specExpires = g_admin_namelog[ i ]->specExpires;
            G_AdminsPrintf( "^7%s^7's Putteam spectator has been restored\n", client->pers.netname );
            g_admin_namelog[ i ]->specExpires = 0;
        }
        */
      }
      else
      {
        //for mute
        if( client->pers.muted )
        {
            g_admin_namelog[ i ]->muted = qtrue;
            //g_admin_namelog[ i ]->muteExpires = client->pers.muteExpires;
        }
        //denybuild
        if( client->pers.denyBuild )
        {
            g_admin_namelog[ i ]->denyBuild = qtrue;
        }
        //denyweapon humans
        if( client->pers.denyHumanWeapons > 0 )
        {
            g_admin_namelog[ i ]->denyHumanWeapons = client->pers.denyHumanWeapons;
        }
        //denyweapon aliens
        if( client->pers.denyAlienClasses > 0 )
        {
            g_admin_namelog[ i ]->denyAlienClasses = client->pers.denyAlienClasses;
        }
        //forcespec
        g_admin_namelog[ i ]->forcespeced = client->pers.specd;
      }

      return;
    }
  }
  if( i >= MAX_ADMIN_NAMELOGS )
  {
    G_Printf( "G_admin_namelog_update: warning, g_admin_namelogs overflow\n" );
    return;
  }
  namelog = G_Alloc( sizeof( g_admin_namelog_t ) );
  memset( namelog, 0, sizeof( namelog ) );
  for( j = 0; j < MAX_ADMIN_NAMELOG_NAMES ; j++ )
    namelog->name[ j ][ 0 ] = '\0';
  Q_strncpyz( namelog->ip, client->pers.ip, sizeof( namelog->ip ) );
  Q_strncpyz( namelog->guid, client->pers.guid, sizeof( namelog->guid ) );
  Q_strncpyz( namelog->name[ 0 ], client->pers.netname, sizeof( namelog->name[ 0 ] ) );
  
  if( trap_GeoIP_GetCountryName( client->pers.ip, countryName ) )
      Q_strncpyz( namelog->country, countryName, sizeof( countryName ) );
  else
      Q_strncpyz( namelog->country, "N/N", 4 );
  
  namelog->slot = ( disconnect ) ? -1 : clientNum;
  g_admin_namelog[ i ] = namelog;
}

qboolean G_admin_readconfig( gentity_t *ent, int skiparg )
{
  g_admin_level_t * l = NULL;
  g_admin_admin_t *a = NULL;
  g_admin_ban_t *b = NULL;
  g_admin_command_t *c = NULL;
  int lc = 0, ac = 0, bc = 0, cc = 0;
  fileHandle_t f;
  int len;
  char *cnf, *cnf2;
  char *t;
  qboolean level_open, admin_open, ban_open, command_open;
  int i = 0;

  G_admin_cleanup();

  if( !g_admin.string[ 0 ] )
  {
    ADMP( "^3!readconfig: g_admin is not set, not loading configuration "
      "from a file\n" );
    admin_default_levels();
    return qfalse;
  }

  len = trap_FS_FOpenFile( g_admin.string, &f, FS_READ ) ;
  if( len < 0 )
  {
    ADMP( va( "^3!readconfig: ^7could not open admin config file %s\n",
            g_admin.string ) );
    admin_default_levels();
    return qfalse;
  }
  cnf = G_Alloc( len + 1 );
  cnf2 = cnf;
  trap_FS_Read( cnf, len, f );
  *( cnf + len ) = '\0';
  trap_FS_FCloseFile( f );

  t = COM_Parse( &cnf );
  level_open = admin_open = ban_open = command_open = qfalse;
  while( *t )
  {
    if( !Q_stricmp( t, "[level]" ) ||
         !Q_stricmp( t, "[admin]" ) ||
         !Q_stricmp( t, "[ban]" ) ||
         !Q_stricmp( t, "[command]" ) )
    {

      if( level_open )
        g_admin_levels[ lc++ ] = l;
      else if( admin_open )
        g_admin_admins[ ac++ ] = a;
      else if( ban_open )
        g_admin_bans[ bc++ ] = b;
      else if( command_open )
        g_admin_commands[ cc++ ] = c;
      level_open = admin_open =
                     ban_open = command_open = qfalse;
    }

    if( level_open )
    {
      if( !Q_stricmp( t, "level" ) )
      {
        admin_readconfig_int( &cnf, &l->level );
      }
      else if( !Q_stricmp( t, "name" ) )
      {
        admin_readconfig_string( &cnf, l->name, sizeof( l->name ) );
      }
      else if( !Q_stricmp( t, "flags" ) )
      {
        admin_readconfig_string( &cnf, l->flags, sizeof( l->flags ) );
      }
      else
      {
        ADMP( va( "^3!readconfig: ^7[level] parse error near %s on line %d\n",
                t,
                COM_GetCurrentParseLine() ) );
      }
    }
    else if( admin_open )
    {
      if( !Q_stricmp( t, "name" ) )
      {
        admin_readconfig_string( &cnf, a->name, sizeof( a->name ) );
      }
      else if( !Q_stricmp( t, "guid" ) )
      {
        admin_readconfig_string( &cnf, a->guid, sizeof( a->guid ) );
      }
      else if( !Q_stricmp( t, "level" ) )
      {
        admin_readconfig_int( &cnf, &a->level );
      }
      else if( !Q_stricmp( t, "flags" ) )
      {
        admin_readconfig_string( &cnf, a->flags, sizeof( a->flags ) );
      }
      else if( !Q_stricmp( t, "seen" ) )
      {
          admin_readconfig_int( &cnf, &a->seen );
      }
      else
      {
        ADMP( va( "^3!readconfig: ^7[admin] parse error near %s on line %d\n",
                t,
                COM_GetCurrentParseLine() ) );
      }

    }
    else if( ban_open )
    {
      if( !Q_stricmp( t, "name" ) )
      {
        admin_readconfig_string( &cnf, b->name, sizeof( b->name ) );
      }
      else if( !Q_stricmp( t, "guid" ) )
      {
        admin_readconfig_string( &cnf, b->guid, sizeof( b->guid ) );
      }
      else if( !Q_stricmp( t, "ip" ) )
      {
        admin_readconfig_string( &cnf, b->ip, sizeof( b->ip ) );
      }
      else if( !Q_stricmp( t, "reason" ) )
      {
        admin_readconfig_string( &cnf, b->reason, sizeof( b->reason ) );
      }
      else if( !Q_stricmp( t, "made" ) )
      {
        admin_readconfig_string( &cnf, b->made, sizeof( b->made ) );
      }
      else if( !Q_stricmp( t, "expires" ) )
      {
        admin_readconfig_int( &cnf, &b->expires );
      }
      else if( !Q_stricmp( t, "suspend" ) )
      {
        admin_readconfig_int( &cnf, &b->suspend );
      }
      else if( !Q_stricmp( t, "banner" ) )
      {
        admin_readconfig_string( &cnf, b->banner, sizeof( b->banner ) );
      }
      else
      {
        ADMP( va( "^3!readconfig: ^7[ban] parse error near %s on line %d\n",
                t,
                COM_GetCurrentParseLine() ) );
      }
    }
    else if( command_open )
    {
      if( !Q_stricmp( t, "command" ) )
      {
        admin_readconfig_string( &cnf, c->command, sizeof( c->command ) );
      }
      else if( !Q_stricmp( t, "exec" ) )
      {
        admin_readconfig_string( &cnf, c->exec, sizeof( c->exec ) );
      }
      else if( !Q_stricmp( t, "desc" ) )
      {
        admin_readconfig_string( &cnf, c->desc, sizeof( c->desc ) );
      }
      else if( !Q_stricmp( t, "flag" ) )
      {
          admin_readconfig_string( &cnf, c->flag, sizeof( c->flag ) );
      }
      else
      {
        ADMP( va( "^3!readconfig: ^7[command] parse error near %s on line %d\n",
                t,
                COM_GetCurrentParseLine() ) );
      }
    }

    if( !Q_stricmp( t, "[level]" ) )
    {
      if( lc >= MAX_ADMIN_LEVELS )
        return qfalse;
      l = G_Alloc( sizeof( g_admin_level_t ) );
      l->level = 0;
      *l->name = '\0';
      *l->flags = '\0';
      level_open = qtrue;
    }
    else if( !Q_stricmp( t, "[admin]" ) )
    {
      if( ac >= MAX_ADMIN_ADMINS )
        return qfalse;
      a = G_Alloc( sizeof( g_admin_admin_t ) );
      *a->name = '\0';
      *a->guid = '\0';
      a->level = 0;
      *a->flags = '\0';
      a->seen = 0;
      admin_open = qtrue;
    }
    else if( !Q_stricmp( t, "[ban]" ) )
    {
      if( bc >= MAX_ADMIN_BANS )
        return qfalse;
      b = G_Alloc( sizeof( g_admin_ban_t ) );
      *b->name = '\0';
      *b->guid = '\0';
      *b->ip = '\0';
      *b->made = '\0';
      b->expires = 0;
      b->suspend = 0;
      *b->reason = '\0';
      ban_open = qtrue;
    }
    else if( !Q_stricmp( t, "[command]" ) )
    {
      if( cc >= MAX_ADMIN_COMMANDS )
        return qfalse;
      c = G_Alloc( sizeof( g_admin_command_t ) );
      *c->command = '\0';
      *c->exec = '\0';
      *c->desc = '\0';
      *c->flag = '\0';
      command_open = qtrue;
    }
    t = COM_Parse( &cnf );
  }
  if( level_open )
  {

    g_admin_levels[ lc++ ] = l;
  }
  if( admin_open )
    g_admin_admins[ ac++ ] = a;
  if( ban_open )
    g_admin_bans[ bc++ ] = b;
  if( command_open )
    g_admin_commands[ cc++ ] = c;
  G_Free( cnf2 );
  ADMP( va( "^3!readconfig: ^7loaded %d levels, %d admins, %d bans, %d commands\n",
          lc, ac, bc, cc ) );
  if( lc == 0 )
    admin_default_levels();
  else
  {
    char n[ MAX_NAME_LENGTH ] = {""};
 
    // max printable name length for formatting 
    for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
    {
      G_DecolorString( l->name, n );
      if( strlen( n ) > admin_level_maxname )
        admin_level_maxname = strlen( n );
    }
  }

// reset adminLevel
for( i = 0; i < level.maxclients; i++ )
    if( level.clients[ i ].pers.connected != CON_DISCONNECTED )
        level.clients[ i ].pers.adminLevel = G_admin_level( &g_entities[ i ] );

  return qtrue;
}

qboolean G_admin_time( gentity_t *ent, int skiparg )
{
  qtime_t qt;

  trap_RealTime( &qt );
  ADMP( va( "^3!time: ^7local time is %02i:%02i:%02i\n", qt.tm_hour, qt.tm_min, qt.tm_sec ) );
  
  return qtrue;
}

static int G_admin_find_slot( gentity_t *ent, char *namearg, const char *command )
{
  char name[ MAX_NAME_LENGTH ];
  char testname[ MAX_NAME_LENGTH ];
  char *guid = NULL;
  int matches = 0;
  int id = -1;
  int i;
  qboolean numeric = qtrue;

  G_SanitiseString( namearg, name, sizeof( name ) );
  for( i = 0; i < sizeof( name ) && name[ i ] ; i++ )
  {
    if( !isdigit( name[ i ] ) )
    {
      numeric = qfalse;
      break;
    }
  }
  if( numeric )
  {
    id = atoi( name );

    if( id >= 0 && id < level.maxclients )
    {
      gentity_t *vic;

      vic = &g_entities[ id ];
      if( vic && vic->client && vic->client->pers.connected != CON_DISCONNECTED )
        return id;

      ADMP( va( "^3!%s:^7 no one connected by that slot number\n", command ) );
      return -1;
    }
    if( id >= MAX_CLIENTS && id < MAX_CLIENTS + MAX_ADMIN_ADMINS &&
        g_admin_admins[ id - MAX_CLIENTS ] )
    {
      return id;
    }

    ADMP( va( "^3!%s:^7 no match for slot or admin number %d\n", command, id ) );
    return -1;
  }

  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ] && matches < 2; i++ )
  {
    G_SanitiseString( g_admin_admins[ i ]->name, testname, sizeof( testname ) );
    if( strstr( testname, name ) )
    {
      id = i + MAX_CLIENTS;
      guid = g_admin_admins[ i ]->guid;
      matches++;
    }
  }
  for( i = 0; i < level.maxclients && matches < 2; i++ )
  {
    if( level.clients[ i ].pers.connected == CON_DISCONNECTED )
      continue;

    if( matches && guid && !Q_stricmp( level.clients[ i ].pers.guid, guid ) )
      continue;

    G_SanitiseString( level.clients[ i ].pers.netname, testname, sizeof( testname ) );
    if( strstr( testname, name ) )
    {
      id = i;
      matches++;
    }
  }

  if( matches == 0 )
  {
    ADMP( va( "^3!%s:^7 no match, use !listplayers or !listadmins to "
      "find an appropriate number to use instead of name.\n", command ) );
    return -1;
  }

  if( matches == 1 )
    return id;

  ADMP( va( "^3!%s:^7 multiple matches, use the admin number instead:\n", command ) );
  admin_listadmins( ent, 0, name, 0 );

  return -1;
}

static int G_admin_find_admin_slot( gentity_t *ent, char *namearg, char *command, char *nick, int nick_len )
{
  gentity_t *vic;
  char *guid;
  int id;
  int i;

  if ( nick )
    nick[ 0 ] = '\0';

  id = G_admin_find_slot( ent, namearg, command );
  if( id < 0 )
    return -1;

  if( id < MAX_CLIENTS )
  {
    vic = &g_entities[ id ];
    guid = vic->client->pers.guid;
    for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
    {
      if( !Q_stricmp( guid, g_admin_admins[ i ]->guid ) )
      {
        id = i + MAX_CLIENTS;
        if( nick )
          Q_strncpyz( nick, vic->client->pers.netname, nick_len );
        break;
      }
    }
    if( id < MAX_CLIENTS )
    {
      ADMP( va( "^3!%s:^7 player is not !registered\n", command ) );
      return -1;
    }
  }

  id -= MAX_CLIENTS;
  if( nick && !nick[ 0 ] )
    Q_strncpyz( nick, g_admin_admins[ id ]->name, nick_len );

  return id;
}

qboolean G_admin_setlevel( gentity_t *ent, int skiparg )
{
  char name[ MAX_NAME_LENGTH ] = {""};
  char lstr[ 11 ]; // 10 is max strlen() for 32-bit int
  char adminname[ MAX_NAME_LENGTH ] = {""};
  char testname[ MAX_NAME_LENGTH ] = {""};
  char testname2[ MAX_NAME_LENGTH ] = {""};
  char guid[ 33 ];
  int l, i, j;
  gentity_t *vic = NULL;
  qboolean updated = qfalse;
  g_admin_admin_t *a;
  qboolean found = qfalse;
  qboolean numeric = qtrue;
  int matches = 0;
  int id = -1;


  if( G_SayArgc() < 3 + skiparg )
  {
    ADMP( "^3!setlevel: ^7usage: !setlevel [name|slot#] [level]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, testname, sizeof( testname ) );
  G_SayArgv( 2 + skiparg, lstr, sizeof( lstr ) );
  l = atoi( lstr );
  G_SanitiseString( testname, name, sizeof( name ) );
  for( i = 0; i < sizeof( name ) && name[ i ] ; i++ )
  {
    if( name[ i ] < '0' || name[ i ] > '9' )
    {
      numeric = qfalse;
      break;
    }
  }
  if( numeric )
    id = atoi( name );

  if( ent && l > ent->client->pers.adminLevel )
  {
    ADMP( "^3!setlevel: ^7you may not use !setlevel to set a level higher "
      "than your current level\n" );
    return qfalse;
  }

  // if admin is activated for the first time on a running server, we need
  // to ensure at least the default levels get created
  if( !ent && !g_admin_levels[ 0 ] )
    G_admin_readconfig(NULL, 0);

  for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
  {
    if( g_admin_levels[ i ]->level == l )
    {
      found = qtrue;
      break;
    }
  }
  if( !found )
  {
    ADMP( "^3!setlevel: ^7level is not defined\n" );
    return qfalse;
  }

  if( numeric  && id >= 0 && id < level.maxclients )
    vic = &g_entities[ id ];

  if( vic && vic->client && vic->client->pers.connected == CON_CONNECTED ) 
  {
    vic = &g_entities[ id ];
    Q_strncpyz( adminname, vic->client->pers.netname, sizeof( adminname ) );
    Q_strncpyz( guid, vic->client->pers.guid, sizeof( guid ) );
    matches = 1;
  }
  else if( numeric && id >= MAX_CLIENTS && id < MAX_CLIENTS + MAX_ADMIN_ADMINS
    && g_admin_admins[ id - MAX_CLIENTS ] )
  {
    Q_strncpyz( adminname, g_admin_admins[ id - MAX_CLIENTS ]->name,
      sizeof( adminname ) );
    Q_strncpyz( guid, g_admin_admins[ id - MAX_CLIENTS ]->guid,
      sizeof( guid ) );
    matches = 1;
  }
  else
  {
    for( i = 0; i < level.maxclients && matches < 2; i++ )
    {
      vic = &g_entities[ i ];
      if( !vic->client || vic->client->pers.connected != CON_CONNECTED )
        continue;
      G_SanitiseString( vic->client->pers.netname, testname, sizeof( testname ) );
      if( strstr( testname, name ) )
      {
        matches++;
        Q_strncpyz( adminname, vic->client->pers.netname, sizeof( adminname ) );
        Q_strncpyz( guid, vic->client->pers.guid, sizeof( guid ) );
      }
    }
    for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ] && matches < 2; i++)
    {
      G_SanitiseString( g_admin_admins[ i ]->name, testname, sizeof( testname ) );
      if( strstr( testname, name ) )
      {
        qboolean dup = qfalse;

        // verify we don't have the same guid/name pair in connected players
        for( j = 0; j < level.maxclients; j++ )
        {
          vic = &g_entities[ j ];
          if( !vic->client || vic->client->pers.connected != CON_CONNECTED )
            continue;
          G_SanitiseString(  vic->client->pers.netname, testname2, sizeof( testname2 ) );
          if( !Q_stricmp( vic->client->pers.guid, g_admin_admins[ i ]->guid )
            && strstr( testname2, name ) ) 
          {
            dup = qtrue;
            break;
          }
        }
        if( dup )
          continue;
        Q_strncpyz( adminname, g_admin_admins[ i ]->name, sizeof( adminname ) );
        Q_strncpyz( guid, g_admin_admins[ i ]->guid, sizeof( guid ) );
        matches++;
      }
    }
  }

  if( matches == 0 )
  {
    ADMP( "^3!setlevel:^7 no match.  use !listplayers or !listadmins to "
      "find an appropriate number to use instead of name.\n" );
    return qfalse;
  }
  else if( matches > 1 )
  {
    ADMP( "^3!setlevel:^7 more than one match.  Use the admin number "
      "instead:\n" );
    admin_listadmins( ent, 0, name, 0 );
    return qfalse;
  }

  if( !Q_stricmp( guid, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" ) )
  {
    ADMP( va( "^3!setlevel: ^7%s does not have a valid GUID\n", adminname ) );
    return qfalse;
  }
  if( ent && !admin_higher_guid( ent->client->pers.guid, guid ) )
  {
    ADMP( "^3!setlevel: ^7sorry, but your intended victim has a higher"
        " admin level than you\n" );
    return qfalse;
  }

  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ];i++ )
  {
    if( !Q_stricmp( g_admin_admins[ i ]->guid, guid ) )
    {
      g_admin_admins[ i ]->level = l;
      Q_strncpyz( g_admin_admins[ i ]->name, adminname,
                  sizeof( g_admin_admins[ i ]->name ) );
      updated = qtrue;
    }
  }
  if( !updated )
  {
    if( i == MAX_ADMIN_ADMINS )
    {
      ADMP( "^3!setlevel: ^7too many admins\n" );
      return qfalse;
    }
    a = G_Alloc( sizeof( g_admin_admin_t ) );
    a->level = l;
    Q_strncpyz( a->name, adminname, sizeof( a->name ) );
    Q_strncpyz( a->guid, guid, sizeof( a->guid ) );
    *a->flags = '\0';
    g_admin_admins[ i ] = a;
  }

  AP( va( 
    "print \"^3!setlevel: ^7%s^7 was given level %d admin rights by %s\n\"",
    adminname, l, ( ent ) ? ent->client->pers.netname : "console" ) );
  if( vic )
    vic->client->pers.adminLevel = l;
  
  if( !g_admin.string[ 0 ] )
    ADMP( "^3!setlevel: ^7WARNING g_admin not set, not saving admin record "
      "to a file\n" );
  else
    G_admin_writeconfig( );
  return qtrue;
}

static void admin_check_duplicate_ban( int ban )
{
  qtime_t qt;
  int t;
  int i, j;
  qboolean immune;

  if ( ban < 0 || ban >= MAX_ADMIN_BANS || !g_admin_bans[ ban ] )
    return;

  t = trap_RealTime( &qt );
  for( i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++ )
  {
    if( g_admin_bans[ i ]->expires != 0
      && ( g_admin_bans[ i ]->expires - t ) < 1 )
    {
      continue;
    }

    if( i != ban &&
      strstr( g_admin_bans[ ban ]->ip, g_admin_bans[ i ]->ip ) == g_admin_bans[ ban ]->ip )
    {
      immune = qfalse;

      for( j = 0; j < MAX_ADMIN_ADMINS && g_admin_admins[ j ]; j++ )
      {
        if( !Q_stricmp( g_admin_bans[ ban ]->guid, g_admin_admins[ j ]->guid ) &&
            G_admin_permission_guid( g_admin_bans[ j ]->guid, ADMF_BAN_IMMUNITY ) )
        {
          immune = qtrue;
        }
      }

      G_AdminsPrintf( "ban #%d duplicates %sban #%d%s.\n",
        ban + 1,
        ( g_admin_bans[ i ]->suspend > t ) ? "SUSPENDED " : "",
        i + 1,
        ( immune ) ? ", player has immunity" : "" );
    }
  }
}

static qboolean admin_create_ban( gentity_t *ent,
  char *netname,
  char *guid,
  char *ip,
  int seconds,
  char *reason ) 
{
  g_admin_ban_t *b = NULL;
  qtime_t qt;
  int t;
  int i;
  int j;
  qboolean foundAdminTrueName=qfalse;

  t = trap_RealTime( &qt );
  b = G_Alloc( sizeof( g_admin_ban_t ) );

  if( !b )
    return qfalse;

  Q_strncpyz( b->name, netname, sizeof( b->name ) );
  Q_strncpyz( b->guid, guid, sizeof( b->guid ) );
  Q_strncpyz( b->ip, ip, sizeof( b->ip ) );
  b->suspend = 0;

  Q_strncpyz( b->made, va( "%02i/%02i/%02i %02i:%02i:%02i",
    (qt.tm_mon + 1), qt.tm_mday, (qt.tm_year - 100),
    qt.tm_hour, qt.tm_min, qt.tm_sec ),
    sizeof( b->made ) );

  if( ent ) {
    //Get admin true name
    for(j = 0; j < MAX_ADMIN_ADMINS && g_admin_admins[ j ]; j++ )
    {
      if( !Q_stricmp( g_admin_admins[ j ]->guid, ent->client->pers.guid ) )
      {
          Q_strncpyz( b->banner, g_admin_admins[ j ]->name, sizeof( b->banner  ) );
	  foundAdminTrueName=qtrue;
        break;
      }
    }
    if(foundAdminTrueName==qfalse) Q_strncpyz( b->banner, ent->client->pers.netname, sizeof( b->banner ) );
  }
  else
    Q_strncpyz( b->banner, "console", sizeof( b->banner ) );
  if( !seconds )
    b->expires = 0;
  else
    b->expires = t + seconds;
  if( !*reason )
    Q_strncpyz( b->reason, "banned by admin", sizeof( b->reason ) );
  else
    Q_strncpyz( b->reason, reason, sizeof( b->reason ) );
  for( i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++ )
    ;
  if( i == MAX_ADMIN_BANS )
  {
    ADMP( "^3!ban: ^7too many bans\n" );
    G_Free( b );
    return qfalse;
  }
  g_admin_bans[ i ] = b;

  admin_check_duplicate_ban( i );

  return qtrue;
}


qboolean G_admin_kick( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], *reason, err[ MAX_STRING_CHARS ];
  int minargc;
  gentity_t *vic;

  minargc = 3 + skiparg;
  if( G_admin_permission( ent, ADMF_UNACCOUNTABLE ) )
    minargc = 2 + skiparg;

  if( G_SayArgc() < minargc )
  {
    ADMP( "^3!kick: ^7usage: !kick [name] [reason]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  reason = G_SayConcatArgs( 2 + skiparg );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!kick: ^7%s\n", err ) );
    return qfalse;
  }
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( "^3!kick: ^7sorry, but your intended victim has a higher admin"
        " level than you\n" );
    return qfalse;
  }
  vic = &g_entities[ pids[ 0 ] ];
  if( g_adminTempBan.integer > 0 )
  {
    admin_create_ban( ent,
      vic->client->pers.netname,
      vic->client->pers.guid,
      vic->client->pers.ip, g_adminTempBan.integer,
      ( *reason ) ? reason : "automatic temp ban created by kick" );
    if( g_admin.string[ 0 ] )
      G_admin_writeconfig( );
  }
  
  trap_SendServerCommand( pids[ 0 ],
    va( "disconnect \"You have been kicked.\n%s^7\nreason:\n%s\"",
      ( ent ) ? va( "admin:\n%s", ent->client->pers.netname ) : "",
      ( *reason ) ? reason : "kicked by admin" ) );

  trap_DropClient( pids[ 0 ], va( "has been kicked%s^7. reason: %s",
    ( ent ) ? va( " by %s", ent->client->pers.netname ) : "",
    ( *reason ) ? reason : "kicked by admin" ) );

  return qtrue;
}

int G_admin_parse_time( const char *time )
{
  int seconds = 0, num = 0;
  int i;

  for( i = 0; time[ i ]; i++ )
  {
    if( time[ i ] >= '0' && time[ i ] <= '9' )
    {
      num = num * 10 + time[ i ] - '0';
      continue;
    }
    switch( time[ i ] )
    {
      case 'w': num *= 7;
      case 'd': num *= 24;
      case 'h': num *= 60;
      case 'm': num *= 60;
      case 's': break;
      default:  return -1;
    }
    seconds += num;
    num = 0;
  }
  if (num)
    seconds += num;
  return seconds;
}

qboolean G_admin_ban( gentity_t *ent, int skiparg )
{
  int seconds;
  char search[ MAX_NAME_LENGTH ];
  char secs[ 7 ];
  char *reason;
  int minargc;
  char duration[ 32 ];
  int logmatch = -1, logmatches = 0;
  int i, j;
  qboolean exactmatch = qfalse;
  char n2[ MAX_NAME_LENGTH ];
  char s2[ MAX_NAME_LENGTH ];
  char guid_stub[ 9 ];
  char notice[51];
  
  trap_Cvar_VariableStringBuffer( "g_banNotice", notice, sizeof( notice ) );
  
  if( G_admin_permission( ent, ADMF_CAN_PERM_BAN ) &&
       G_admin_permission( ent, ADMF_UNACCOUNTABLE ) )
  {
    minargc = 2 + skiparg;
  }
  else if( G_admin_permission( ent, ADMF_CAN_PERM_BAN ) ||
            G_admin_permission( ent, ADMF_UNACCOUNTABLE ) )
  {
    minargc = 3 + skiparg;
  }
  else
  {
    minargc = 4 + skiparg;
  }
  if( G_SayArgc() < minargc )
  {
    ADMP( "^3!ban: ^7usage: !ban [name|slot|ip] [seconds] [reason]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, search, sizeof( search ) );
  G_SanitiseString( search, s2, sizeof( s2 ) );
  G_SayArgv( 2 + skiparg, secs, sizeof( secs ) );

  seconds = G_admin_parse_time( secs );
  if( seconds <= 0 )
  {
    if( g_adminMaxBan.integer )
    {
      ADMP( va( "^3!ban: ^7using default ban length of %d days\n",
        g_adminMaxBan.integer ) );
      seconds = g_adminMaxBan.integer * 60 * 60 * 24;
    }
    else if( G_admin_permission( ent, ADMF_CAN_PERM_BAN ) )
    {
      seconds = 0;
    }
    else
    {
      ADMP( "^3!ban: ^7ban time must be positive\n" );
      return qfalse;
    }
    reason = G_SayConcatArgs( 2 + skiparg );
  }
  else
  {
    reason = G_SayConcatArgs( 3 + skiparg );

    if( g_adminMaxBan.integer &&
        seconds > g_adminMaxBan.integer * 60 * 60 * 24 &&
        !G_admin_permission( ent, ADMF_CAN_PERM_BAN ) )
    {
      seconds = g_adminMaxBan.integer * 60 * 60 * 24;
      ADMP( va( "^3!ban: ^7ban length limited to %d days for your admin level\n",
        g_adminMaxBan.integer ) );
    }
  }

  for( i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++ )
  {
    // skip players in the namelog who have already been banned
    if( g_admin_namelog[ i ]->banned )
      continue;

    // skip disconnected players when banning on slot number
    if( g_admin_namelog[ i ]->slot == -1 )
      continue;

    if( !Q_stricmp( va( "%d", g_admin_namelog[ i ]->slot ), s2 ) )
    {
      logmatches = 1;
      logmatch = i;
      exactmatch = qtrue;
      break;
    }
  } 

  for( i = 0;
       !exactmatch && i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ];
       i++ )
  {
    // skip players in the namelog who have already been banned
    if( g_admin_namelog[ i ]->banned )
      continue;

    if( !Q_stricmp( g_admin_namelog[ i ]->ip, s2 ) )
    {
      logmatches = 1;
      logmatch = i;
      exactmatch = qtrue;
      break;
    }
    for( j = 0; j < MAX_ADMIN_NAMELOG_NAMES
      && g_admin_namelog[ i ]->name[ j ][ 0 ]; j++ )
    {
      G_SanitiseString(g_admin_namelog[ i ]->name[ j ], n2, sizeof( n2 ) );
      if( strstr( n2, s2 ) )
      {
        if( logmatch != i )
          logmatches++;
        logmatch = i; 
      }
    }
  }
  
  if( !logmatches ) 
  {
    ADMP( "^3!ban: ^7no player found by that name, IP, or slot number\n" );
    return qfalse;
  } 
  else if( logmatches > 1 )
  {
    ADMBP_begin();
    ADMBP( "^3!ban: ^7multiple recent clients match name, use IP or slot#:\n" );
    for( i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++ )
    {
      for( j = 0; j < 8; j++ )
        guid_stub[ j ] = g_admin_namelog[ i ]->guid[ j + 24 ];
      guid_stub[ j ] = '\0';
      for( j = 0; j < MAX_ADMIN_NAMELOG_NAMES
        && g_admin_namelog[ i ]->name[ j ][ 0 ]; j++ )
      {
        G_SanitiseString( g_admin_namelog[ i ]->name[ j ], n2, sizeof( n2 ) );
        if( strstr( n2, s2 ) )
        {
          if( g_admin_namelog[ i ]->slot > -1 )
            ADMBP( "^3" );
          ADMBP( va( "%-2s (*%s) %15s ^7'%s^7'\n",
           (g_admin_namelog[ i ]->slot > -1) ?
             va( "%d", g_admin_namelog[ i ]->slot ) : "-",
           guid_stub,
           g_admin_namelog[ i ]->ip,
           g_admin_namelog[ i ]->name[ j ] ) );
        }
      }
    }
    ADMBP_end();
    return qfalse;
  }
  
  G_admin_duration( ( seconds ) ? seconds : -1,
    duration, sizeof( duration ) );

  if( ent && !admin_higher_guid( ent->client->pers.guid,
    g_admin_namelog[ logmatch ]->guid ) )
  {

    ADMP( "^3!ban: ^7sorry, but your intended victim has a higher admin"
      " level than you\n" );
    return qfalse;
  }

  admin_create_ban( ent,
    g_admin_namelog[ logmatch ]->name[ 0 ],
    g_admin_namelog[ logmatch ]->guid,
    g_admin_namelog[ logmatch ]->ip,
    seconds, reason ); 

  g_admin_namelog[ logmatch ]->banned = qtrue;

  if( !g_admin.string[ 0 ] )
    ADMP( "^3!ban: ^7WARNING g_admin not set, not saving ban to a file\n" );
  else
    G_admin_writeconfig( );

  if(g_admin_namelog[ logmatch ]->slot == -1 ) 
  {
    // client is already disconnected so stop here
    AP( va( "print \"^3!ban:^7 %s^7 has been banned by %s^7 "
      "duration: %s, reason: %s\n\"",
      g_admin_namelog[ logmatch ]->name[ 0 ],
      ( ent ) ? ent->client->pers.netname : "console",
      duration,
      ( *reason ) ? reason : "banned by admin" ) );
    return qtrue;
  }

  trap_SendServerCommand( g_admin_namelog[ logmatch ]->slot,
    va( "disconnect \"You have been banned.\n"
      "admin:\n%s^7\nduration:\n%s\nreason:\n%s\n%s\"",
      ( ent ) ? ent->client->pers.netname : "console",
      duration,
      ( *reason ) ? reason : "kicked by admin", notice ) );

  trap_DropClient(  g_admin_namelog[ logmatch ]->slot,
    va( "has been banned by %s^7 duration: %s, reason: %s",
      ( ent ) ? ent->client->pers.netname : "console",
      duration,
      ( *reason ) ? reason : "banned by admin" ) );
  return qtrue;
}

qboolean G_admin_adjustban( gentity_t *ent, int skiparg )
{
  int bnum;
  int length;
  char *arg;
  char bs[ 5 ];

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!adjustban: ^7usage: !adjustban [ban #] [length|reason]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, bs, sizeof( bs ) );
  bnum = atoi( bs );
  if( bnum < 1 || !g_admin_bans[ bnum - 1] )
  {
    ADMP( "^3!adjustban: ^7invalid ban #\n" );
    return qfalse;
  }

  arg = G_SayConcatArgs( 2 + skiparg );
  length = G_admin_parse_time( arg );
  if( G_SayArgc() > 3 + skiparg || length < 0) // reason
  {
    Q_strncpyz( g_admin_bans[ bnum - 1 ]->reason, arg,
      sizeof( g_admin_bans[ bnum - 1 ]->reason ) );
    AP( va( "print \"^3!adjustban: ^7ban #%d reason changed to %s\n\"", bnum,
            arg ) );
  }
  else // time
  {
    int expires;
    char duration[ 32 ];
    if( length )
    {
      qtime_t qt;

      if( g_adminMaxBan.integer &&
          !G_admin_permission( ent, ADMF_CAN_PERM_BAN ) &&
          length > g_adminMaxBan.integer * 60 * 60 * 24 )
      {
        ADMP( va( "^3!adjustban: ^7ban length is limited to %d days for your admin level\n",
          g_adminMaxBan.integer ) );
        return qfalse;
      }

      expires = trap_RealTime( &qt ) + length;
    }
    else
    {
      if( !G_admin_permission( ent, ADMF_CAN_PERM_BAN ) )
      {
        ADMP( "^3adjustban: ^7permanent bans disabled for your admin level\n" );
        return qfalse;
      }
      expires = 0;
    }

    if( g_admin_bans[ bnum - 1 ]->expires == expires )
    {
      ADMP( "^3!adjustban: ^7no change\n" );
      return qfalse;
    }
    g_admin_bans[ bnum - 1 ]->expires = expires;
    G_admin_duration( expires - ( expires - length ), duration, sizeof( duration ) );

    AP( va( "print \"^3!adjustban: ^7ban #%d length adjusted to %s\n\"", bnum,
            duration ) );
  }

  if( !g_admin.string[ 0 ] )
    ADMP( "^3!adjustban: ^7WARNING g_admin not set, not saving ban to a file\n" );
  else
    G_admin_writeconfig( );
  return qtrue;
}

qboolean G_admin_unban( gentity_t *ent, int skiparg )
{
  int bnum;
  char bs[ 5 ];
  qtime_t qt;
  int t;

  t = trap_RealTime( &qt );
  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!unban: ^7usage: !unban [ban #]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, bs, sizeof( bs ) );
  bnum = atoi( bs );
  if( bnum < 1 || !g_admin_bans[ bnum - 1 ] )
  {
    ADMP( "^3!unban: ^7invalid ban #\n" );
    return qfalse;
  }
  g_admin_bans[ bnum -1 ]->expires = t;
  AP( va( "print \"^3!unban: ^7ban #%d for %s^7 has been removed by %s\n\"",
          bnum,
          g_admin_bans[ bnum - 1 ]->name,
          ( ent ) ? ent->client->pers.netname : "console" ) );
  if( g_admin.string[ 0 ] )
    G_admin_writeconfig( );
  return qtrue;
}

qboolean G_admin_putteam( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], team[ 7 ];
  gentity_t *vic;
  pTeam_t teamnum = PTE_NONE;
  char teamdesc[ 32 ] = {"spectators"};
  int pcount = 0;
  int i;

  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  G_SayArgv( 2 + skiparg, team, sizeof( team ) );
  if( G_SayArgc() < 3 + skiparg )
  {
    ADMP( "^3!putteam: ^7usage: !putteam [name] [h|a|s]\n" );
    return qfalse;
  }

  pcount = G_ClientNumbersFromString( name, pids );
  if( pcount < 1 )
  {
    ADMP( va( "^3!putteam: ^7%s\n", "No players found with that name." ) );
    return qfalse;
  }

  switch( team[ 0 ] )
  {
  case 'a':
    teamnum = PTE_ALIENS;
    Q_strncpyz( teamdesc, "aliens", sizeof( teamdesc ) );
    break;
  case 'h':
    teamnum = PTE_HUMANS;
    Q_strncpyz( teamdesc, "humans", sizeof( teamdesc ) );
    break;
  case 's':
    teamnum = PTE_NONE;
    break;
  default:
    ADMP( va( "^3!putteam: ^7unknown team %c\n", team[ 0 ] ) );
    return qfalse;
  }

  for( i = 0; i < pcount; i++ )
  {
    vic = &g_entities[ pids[ i ] ];

    if( !admin_higher( ent, vic ) )
    {
      ADMP( "^3!putteam: ^7sorry, but your intended victim has a higher "
            " admin level than you\n" );
      continue;
    }

    //The player is already on that team
    if( vic->client->pers.teamSelection == teamnum )
        continue;

    if( level.demoState == DS_PLAYBACK )
    {
      ADMP( "^3!putteam: ^7cannot join a team while a demo is playing\n" );
      return qfalse;
    }

    if( vic->client->sess.invisible == qtrue ) {
        ADMP( "^3!putteam: ^7invisible players cannot join a team\n" );
        return qfalse;
    }

    G_ChangeTeam( vic, teamnum );

    AP( va( "print \"^3!putteam: ^7%s^7 put %s^7 on to the %s team\n\"",
            ( ent ) ? ent->client->pers.netname : "console",
            vic->client->pers.netname, teamdesc ) );
  }

  return qtrue;
}

qboolean G_admin_map( gentity_t *ent, int skiparg )
{
  char map[ MAX_QPATH ];
  char layout[ MAX_QPATH ] = { "" };
  
  if( G_SayArgc( ) < 2 + skiparg )
  {
    ADMP( "^3!map: ^7usage: !map [map] (layout)\n" );
    return qfalse;
  }

  G_SayArgv( skiparg + 1, map, sizeof( map ) );

  if( !trap_FS_FOpenFile( va( "maps/%s.bsp", map ), NULL, FS_READ ) )
  {
    ADMP( va( "^3!map: ^7invalid map name '%s'\n", map ) );
    return qfalse;
  }

  if( G_SayArgc( ) > 2 + skiparg )
  {
    G_SayArgv( skiparg + 2, layout, sizeof( layout ) );
    if( !Q_stricmp( layout, "*BUILTIN*" ) ||
      trap_FS_FOpenFile( va( "layouts/%s/%s.dat", map, layout ),
        NULL, FS_READ ) > 0 )
    {
      trap_Cvar_Set( "g_layouts", layout );
    }
    else
    {
      ADMP( va( "^3!map: ^7invalid layout name '%s'\n", layout ) );
      return qfalse;
    }
  }

  G_admin_maplog_result( "M" );

  trap_SendConsoleCommand( EXEC_APPEND, va( "map %s", map ) );

  level.restarted = qtrue;
  AP( va( "print \"^3!map: ^7map '%s' started by %s^7 %s\n\"", map,
          ( ent ) ? ent->client->pers.netname : "console",
          ( layout[ 0 ] ) ? va( "(forcing layout '%s')", layout ) : "" ) );

  if( level.scrimmMode )
  {
      G_admin_scrimm_switch( qfalse );
      trap_Cvar_Set( "g_scrimmRestarted", "1" );
      AP( "print \"^3Scrimm ^7mode is ^2ON\n\"" );
  }

  return qtrue;
}

qboolean G_admin_devmap( gentity_t *ent, int skiparg )
{
  char map[ MAX_QPATH ];
  char layout[ MAX_QPATH ] = { "" };

  if( G_SayArgc( ) < 2 + skiparg )
  {
    ADMP( "^3!devmap: ^7usage: !devmap [map] (layout)\n" );
    return qfalse;
  }

  G_SayArgv( skiparg + 1, map, sizeof( map ) );

  if( !trap_FS_FOpenFile( va( "maps/%s.bsp", map ), NULL, FS_READ ) )
  {
    ADMP( va( "^3!devmap: ^7invalid map name '%s'\n", map ) );
    return qfalse;
  }

  if( G_SayArgc( ) > 2 + skiparg )
  {
    G_SayArgv( skiparg + 2, layout, sizeof( layout ) );
    if( !Q_stricmp( layout, "*BUILTIN*" ) ||
      trap_FS_FOpenFile( va( "layouts/%s/%s.dat", map, layout ),
        NULL, FS_READ ) > 0 )
    {
      trap_Cvar_Set( "g_layouts", layout );
    }
    else
    {
      ADMP( va( "^3!devmap: ^7invalid layout name '%s'\n", layout ) );
      return qfalse;
    }
  }

  trap_SendConsoleCommand( EXEC_APPEND, va( "devmap %s", map ) );
  level.restarted = qtrue;
  AP( va( "print \"^3!devmap: ^7map '%s' started by %s^7 with cheats %s\n\"", map,
          ( ent ) ? ent->client->pers.netname : "console",
          ( layout[ 0 ] ) ? va( "(forcing layout '%s')", layout ) : "" ) );
  G_admin_maplog_result( "D" );
  return qtrue;
}

qboolean G_admin_layoutsave( gentity_t *ent, int skiparg )
{
  char layout[ MAX_QPATH ];

  if( G_SayArgc( ) < 2 + skiparg )
  {
    ADMP( "^3!layoutsave: ^7usage: !layoutsave [layout]\n" );
    return qfalse;
  }

  G_SayArgv( skiparg + 1, layout, sizeof( layout ) );

  trap_SendConsoleCommand( EXEC_APPEND, va( "layoutsave %s", layout ) );
  AP( va( "print \"^3!layoutsave: ^7layout saved as '%s' by %s\n\"", layout,
          ( ent ) ? ent->client->pers.netname : "console" ) );
  return qtrue;
}

qboolean G_admin_mute( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  char command[ MAX_ADMIN_CMD_LEN ], *cmd;
  gentity_t *vic;

  G_SayArgv( skiparg, command, sizeof( command ) );
  cmd = command;
  if( cmd && *cmd == '!' )
    cmd++;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( va( "^3!%s: ^7usage: !%s [name|slot#]\n", cmd, cmd ) );
    return qfalse;
  }

  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!%s: ^7%s\n", cmd, err ) );
    return qfalse;
  }
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( va( "^3!%s: ^7sorry, but your intended victim has a higher admin"
              " level than you\n", cmd ) );
    return qfalse;
  }
  vic = &g_entities[ pids[ 0 ] ];
  if( vic->client->pers.muted == qtrue )
  {
    if( !Q_stricmp( cmd, "mute" ) )
    {
      ADMP( "^3!mute: ^7player is already muted\n" );
      return qtrue;
    }

    vic->client->pers.muted = qfalse;

    CPx( pids[ 0 ], "cp \"^1You have been unmuted\"" );
    AP( va( "print \"^3!unmute: ^7%s^7 has been unmuted by %s\n\"",
            vic->client->pers.netname,
            ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  else
  {
    if( !Q_stricmp( cmd, "unmute" ) )
    {
      ADMP( "^3!unmute: ^7player is not currently muted\n" );
      return qtrue;
    }

    vic->client->pers.muted = qtrue;

    CPx( pids[ 0 ], "cp \"^1You've been muted\"" );
    AP( va( "print \"^3!mute: ^7%s^7 has been muted by ^7%s\n\"",
            vic->client->pers.netname,
            ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  ClientUserinfoChanged( pids[ 0 ] );
  return qtrue;
}

qboolean G_admin_denybuild( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  char command[ MAX_ADMIN_CMD_LEN ], *cmd;
  gentity_t *vic;

  G_SayArgv( skiparg, command, sizeof( command ) );
  cmd = command;
  if( cmd && *cmd == '!' )
    cmd++;
  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( va( "^3!%s: ^7usage: !%s [name|slot#]\n", cmd, cmd ) );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!%s: ^7%s\n", cmd, err ) );
    return qfalse;
  }
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( va( "^3!%s: ^7sorry, but your intended victim has a higher admin"
              " level than you\n", cmd ) );
    return qfalse;
  }
  vic = &g_entities[ pids[ 0 ] ];
  if( vic->client->pers.denyBuild )
  {
    if( !Q_stricmp( cmd, "denybuild" ) )
    {
      ADMP( "^3!denybuild: ^7player already has no building rights\n" );
      return qtrue;
    }
    vic->client->pers.denyBuild = qfalse;
    CPx( pids[ 0 ], "cp \"^1You've regained your building rights\"" );
    AP( va(
      "print \"^3!allowbuild: ^7building rights for ^7%s^7 restored by %s\n\"",
      vic->client->pers.netname,
      ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  else
  {
    if( !Q_stricmp( cmd, "allowbuild" ) )
    {
      ADMP( "^3!allowbuild: ^7player already has building rights\n" );
      return qtrue;
    }
    vic->client->pers.denyBuild = qtrue;
    vic->client->ps.stats[ STAT_BUILDABLE ] = BA_NONE;
    if( vic->client->ps.stats[ STAT_PCLASS ] == PCL_ALIEN_BUILDER0 ||
        vic->client->ps.stats[ STAT_PCLASS ] == PCL_ALIEN_BUILDER0_UPG )
    {
      vic->client->ps.stats[ STAT_HEALTH ] = vic->health = 0;
      player_die( vic, vic, vic, 100000, MOD_SUICIDE );
    }
    CPx( pids[ 0 ], "cp \"^1You've lost your building rights\"" );
    AP( va(
      "print \"^3!denybuild: ^7building rights for ^7%s^7 revoked by ^7%s\n\"",
      vic->client->pers.netname,
      ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  ClientUserinfoChanged( pids[ 0 ] );
  return qtrue;
}

qboolean G_admin_denyweapon( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  char command[ MAX_ADMIN_CMD_LEN ], *cmd;
  char buffer[ 32 ];
  int weapon = WP_NONE;
  int class = PCL_NONE;
  char *realname;
  gentity_t *vic;
  int flag;
  qboolean all = qfalse;

  G_SayArgv( skiparg, command, sizeof( command ) );
  cmd = command;
  if( cmd && *cmd == '!' )
    cmd++;
  if( G_SayArgc() < 3 + skiparg )
  {
    ADMP( va( "^3!%s: ^7usage: !%s [name|slot#] [class|weapon]\n", cmd, cmd ) );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!%s: ^7%s\n", cmd, err ) );
    return qfalse;
  }
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( va( "^3!%s: ^7sorry, but your intended victim has a higher admin"
              " level than you\n", cmd ) );
    return qfalse;
  }
  vic = &g_entities[ pids[ 0 ] ];

  G_SayArgv( 2 + skiparg, buffer, sizeof( buffer ) );

  if( !Q_stricmp( buffer, "all" ) &&
      !Q_stricmp( cmd, "allowweapon" ) )
  {
    if( vic->client->pers.denyHumanWeapons ||
        vic->client->pers.denyAlienClasses )
    {
      vic->client->pers.denyHumanWeapons = 0;
      vic->client->pers.denyAlienClasses = 0;

      CPx( pids[ 0 ], "cp \"^1You've regained all weapon and class rights\"" );
      AP( va( "print \"^3!%s: ^7all weapon and class rights for ^7%s^7 restored by %s\n\"",
        cmd,
        vic->client->pers.netname,
        ( ent ) ? ent->client->pers.netname : "console" ) );
    }
    else
    {
      ADMP( va( "^3!%s: ^7player already has all rights\n", cmd ) );
    }
    return qtrue;
  }

  if( !Q_stricmp( buffer, "all" ) &&
      !Q_stricmp( cmd, "denyweapon" ) )
  {
    all = qtrue;
    weapon = WP_NONE;
    class = PCL_NONE;

    if( vic->client->pers.denyHumanWeapons == 0xFFFFFF &&
        vic->client->pers.denyAlienClasses == 0xFFFFFF )
    {
      ADMP( va( "^3!%s: ^7player already has no weapon or class rights\n", cmd ) );
      return qtrue;
    }

    if( vic->client->pers.teamSelection == PTE_HUMANS )
    {
      weapon = vic->client->ps.weapon;
      if( weapon < WP_PAIN_SAW || weapon > WP_GRENADE )
        weapon = WP_NONE;
    }
    if( vic->client->pers.teamSelection == PTE_ALIENS )
    {
      class = vic->client->pers.classSelection;
      if( class < PCL_ALIEN_LEVEL1 || class > PCL_ALIEN_LEVEL4 )
        class = PCL_NONE;
    }

    vic->client->pers.denyHumanWeapons = 0xFFFFFF;
    vic->client->pers.denyAlienClasses = 0xFFFFFF;
  }
  else
  {
    weapon = BG_FindWeaponNumForName( buffer );
    if( weapon < WP_PAIN_SAW || weapon > WP_GRENADE )
      class = BG_FindClassNumForName( buffer );
    if( ( weapon < WP_PAIN_SAW || weapon > WP_GRENADE ) &&
        ( class < PCL_ALIEN_LEVEL1 || class > PCL_ALIEN_LEVEL4 ) )
    {
      {
        ADMP( va( "^3!%s: ^7unknown weapon or class\n", cmd ) );
        return qfalse;
      }
    }
  }

  if( class == PCL_NONE )
  {
    realname = BG_FindHumanNameForWeapon( weapon );
    flag = 1 << (weapon - WP_BLASTER);
    if( !Q_stricmp( cmd, "denyweapon" ) )
    {
      if( ( vic->client->pers.denyHumanWeapons & flag ) && !all )
      {
        ADMP( va( "^3!%s: ^7player already has no %s rights\n", cmd, realname ) );
        return qtrue;
      }
      vic->client->pers.denyHumanWeapons |= flag;
      if( vic->client->pers.teamSelection == PTE_HUMANS )
      {
        if( ( weapon == WP_GRENADE || all ) &&
            BG_InventoryContainsUpgrade( UP_GRENADE, vic->client->ps.stats ) )
        {
          BG_RemoveUpgradeFromInventory( UP_GRENADE, vic->client->ps.stats );
          G_AddCreditToClient( vic->client, (short)BG_FindPriceForUpgrade( UP_GRENADE ), qfalse );
        }
        if( BG_InventoryContainsWeapon( weapon, vic->client->ps.stats ) )
        {
          int maxAmmo, maxClips;

          BG_RemoveWeaponFromInventory( weapon, vic->client->ps.stats );
          G_AddCreditToClient( vic->client, (short)BG_FindPriceForWeapon( weapon ), qfalse );

          BG_AddWeaponToInventory( WP_MACHINEGUN, vic->client->ps.stats );
          BG_FindAmmoForWeapon( WP_MACHINEGUN, &maxAmmo, &maxClips );
          BG_PackAmmoArray( WP_MACHINEGUN, vic->client->ps.ammo, vic->client->ps.powerups,
                            maxAmmo, maxClips );
          G_ForceWeaponChange( vic, WP_MACHINEGUN );
          vic->client->ps.stats[ STAT_MISC ] = 0;
          ClientUserinfoChanged( pids[ 0 ] );
        }
      }
    }
    else
    {
      if( !( vic->client->pers.denyHumanWeapons & flag ) )
      {
        ADMP( va( "^3!%s: ^7player already has %s rights\n", cmd, realname ) );
        return qtrue;
      }
      vic->client->pers.denyHumanWeapons &= ~flag;
    }
  }
  else
  {
    realname = BG_FindHumanNameForClassNum( class );
    flag = 1 << class;
    if( !Q_stricmp( cmd, "denyweapon" ) )
    {
      if( ( vic->client->pers.denyAlienClasses & flag ) && !all )
      {
        ADMP( va( "^3!%s: ^7player already has no %s rights\n", cmd, realname ) );
        return qtrue;
      }
      vic->client->pers.denyAlienClasses |= flag;
      if( vic->client->pers.teamSelection == PTE_ALIENS &&
          vic->client->pers.classSelection == class )
      {
        vec3_t infestOrigin;
        short cost;

        G_RoomForClassChange( vic, PCL_ALIEN_LEVEL0, infestOrigin );

        vic->client->pers.evolveHealthFraction = (float)vic->client->ps.stats[ STAT_HEALTH ] /
            (float)BG_FindHealthForClass( class );
        if( vic->client->pers.evolveHealthFraction < 0.0f )
          vic->client->pers.evolveHealthFraction = 0.0f;
        else if( vic->client->pers.evolveHealthFraction > 1.0f )
          vic->client->pers.evolveHealthFraction = 1.0f;

        vic->client->pers.classSelection = PCL_ALIEN_LEVEL0;
        cost = BG_ClassCanEvolveFromTo( PCL_ALIEN_LEVEL0, class, 9, 0 );
        if( cost < 0 ) cost = 0;
        G_AddCreditToClient( vic->client, cost, qfalse );
        ClientUserinfoChanged( pids[ 0 ] );
        VectorCopy( infestOrigin, vic->s.pos.trBase );
        ClientSpawn( vic, vic, vic->s.pos.trBase, vic->s.apos.trBase );
      }
    }
    else
    {
      if( !( vic->client->pers.denyAlienClasses & flag ) )
      {
        ADMP( va( "^3!%s: ^7player already has %s rights\n", cmd, realname ) );
        return qtrue;
      }
      vic->client->pers.denyAlienClasses &= ~flag;
    }
  }

  if( all )
    realname = "weapon and class";

  CPx( pids[ 0 ], va( "cp \"^1You've %s your %s rights\"",
    ( !Q_stricmp( cmd, "denyweapon" ) ) ? "lost" : "regained",
    realname ) );
  AP( va(
    "print \"^3!%s: ^7%s rights for ^7%s^7 %s by %s\n\"",
    cmd, realname,
    vic->client->pers.netname,
    ( !Q_stricmp( cmd, "denyweapon" ) ) ? "revoked" : "restored",
    ( ent ) ? ent->client->pers.netname : "console" ) );
  
  return qtrue;
}

qboolean G_admin_listadmins( gentity_t *ent, int skiparg )
{
  int i, found = 0;
  qtime_t qt;
  char search[ MAX_NAME_LENGTH ] = {""};
  char s[ MAX_NAME_LENGTH ] = {""};
  int start = 0;
  qboolean numeric = qtrue;
  int drawn = 0;
  int minlevel = 1;

  trap_RealTime( &qt );
  
  if( G_SayArgc() == 3 + skiparg )
  {
    G_SayArgv( 2 + skiparg, s, sizeof( s ) );
    if( s[ 0 ] >= '0' && s[ 0] <= '9' )
    {
      minlevel = atoi( s );
      if( minlevel < 1 ) 
       minlevel = 1;
    }
  }

  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    if( g_admin_admins[ i ]->level >= minlevel )
      found++;
  }
  if( !found )
  {
    if( minlevel > 1 )
    {
      ADMP( va( "^3!listadmins: ^7no admins level %i or greater found\n", minlevel ) );
    }
    else
    {
       ADMP( "^3!listadmins: ^7no admins defined\n" );
    }
    return qfalse;
  }

  if( G_SayArgc() >= 2 + skiparg )
  {
    G_SayArgv( 1 + skiparg, s, sizeof( s ) );
    for( i = 0; i < sizeof( s ) && s[ i ]; i++ )
    {
      if( s[ i ] >= '0' && s[ i ] <= '9' )
        continue;
      numeric = qfalse; 
    }
    if( numeric ) 
    {
      start = atoi( s );
      if( start > 0 )
        start -= 1;
      else if( start < 0 )
        start = found + start;
    }
    else
      G_SanitiseString( s, search, sizeof( search ) );
  }

  if( start >= found || start < 0 )
    start = 0;

  drawn = admin_listadmins( ent, start, search, minlevel );

  if( search[ 0 ] )
  {
    if( drawn <= 20 )
    {
      ADMP( va( "^3!listadmins:^7 found %d admins level %i or greater matching '%s^7'\n",
        drawn, minlevel, search ) );
    }
    else
    {
      ADMP( va( "^3!listadmins:^7 found >20 admins level %i or greater matching '%s^7. Try a more narrow search.'\n",
        minlevel, search ) );
    }
  }
  else
  {
    ADMBP_begin();
    ADMBP( va( "^3!listadmins:^7 showing admins level %i or greater %d - %d of %d.  ",
      minlevel,
      ( found ) ? ( start + 1 ) : 0,
      ( ( start + MAX_ADMIN_LISTITEMS ) > found ) ?
       found : ( start + MAX_ADMIN_LISTITEMS ),
      found ) );
    if( ( start + MAX_ADMIN_LISTITEMS ) < found )
    {
      if( minlevel > 1)
      {
        ADMBP( va( "run '!listadmins %d %d' to see more",
          ( start + MAX_ADMIN_LISTITEMS + 1 ), minlevel ) );
      }
      else
      {
        ADMBP( va( "run '!listadmins %d' to see more",
          ( start + MAX_ADMIN_LISTITEMS + 1 ) ) );
      }
    }
    ADMBP( "\n" );
    ADMBP_end();
  }
  return qtrue;
}

qboolean G_admin_listlayouts( gentity_t *ent, int skiparg )
{
  char list[ MAX_CVAR_VALUE_STRING ];
  char map[ MAX_QPATH ];
  int count = 0;
  char *s;
  char layout[ MAX_QPATH ] = { "" };
  int i = 0;
  
  if( G_SayArgc( ) == 2 + skiparg ) 
    G_SayArgv( 1 +skiparg, map, sizeof( map ) );
  else
    trap_Cvar_VariableStringBuffer( "mapname", map, sizeof( map ) );
  
  count = G_LayoutList( map, list, sizeof( list ) );
  ADMBP_begin( );
  ADMBP( va( "^3!listlayouts:^7 %d layouts found for '%s':\n", count, map ) );
  s = &list[ 0 ];
  while( *s )
  {
    if( *s == ' ' )
    {
      ADMBP( va ( " %s\n", layout ) );
      layout[ 0 ] = '\0';
      i = 0;
    }
    else if( i < sizeof( layout ) - 2 )
    {
      layout[ i++ ] = *s;
      layout[ i ] = '\0';
    }
    s++;
  }
  if( layout[ 0 ] )
    ADMBP( va ( " %s\n", layout ) );
  ADMBP_end( );
  return qtrue;
}

qboolean G_admin_listplayers( gentity_t *ent, int skiparg )
{
  int i, j;
  int invisiblePlayers = 0;
  gclient_t *p;
  char c[ 3 ], t[ 2 ]; // color and team letter
  char n[ MAX_NAME_LENGTH ] = {""};
  char n2[ MAX_NAME_LENGTH ] = {""};
  char n3[ MAX_NAME_LENGTH ] = {""};
  char lname[ MAX_NAME_LENGTH ];
  char lname2[ MAX_NAME_LENGTH ];
  char guid_stub[ 9 ];
  char muted[ 2 ], denied[ 2 ], dbuilder[ 2 ], immune[ 2 ];
  int l;
  char lname_fmt[ 5 ];

  //get amount of invisible players
  for( i = 0; i < level.maxclients; i++ ) {
    p = &level.clients[ i ];
    if ( p->sess.invisible == qtrue )
      invisiblePlayers++;
  }

  ADMBP_begin();
  ADMBP( va( "^3!listplayers^7: %d players connected:\n", level.numConnectedClients - invisiblePlayers ) );

  for( i = 0; i < level.maxclients; i++ )
  {
    p = &level.clients[ i ];

    //ignore invisible players
    if( p->sess.invisible == qtrue )
        continue;

    Q_strncpyz( t, "S", sizeof( t ) );
    Q_strncpyz( c, S_COLOR_YELLOW, sizeof( c ) );
    if( p->pers.teamSelection == PTE_HUMANS )
    {
      Q_strncpyz( t, "H", sizeof( t ) );
      Q_strncpyz( c, S_COLOR_BLUE, sizeof( c ) );
    }
    else if( p->pers.teamSelection == PTE_ALIENS )
    {
      Q_strncpyz( t, "A", sizeof( t ) );
      Q_strncpyz( c, S_COLOR_RED, sizeof( c ) );
    }

    if( p->pers.connected == CON_CONNECTING )
    {
      Q_strncpyz( t, "C", sizeof( t ) );
      Q_strncpyz( c, S_COLOR_CYAN, sizeof( c ) );
    }
    else if( p->pers.connected != CON_CONNECTED )
    {
      continue;
    }

    for( j = 0; j < 8; j++ )
      guid_stub[ j ] = p->pers.guid[ j + 24 ];
    guid_stub[ j ] = '\0';
    
    muted[ 0 ] = '\0';
    if( p->pers.muted )
    {
      Q_strncpyz( muted, "M", sizeof( muted ) );
    }
    else if( p->pers.nlocked )
    {
      Q_strncpyz( muted, "N", sizeof( muted ) );
    }
    denied[ 0 ] = '\0';
    if( p->pers.denyBuild )
    {
      Q_strncpyz( denied, "B", sizeof( denied ) );
    }
    if( p->pers.denyHumanWeapons || p->pers.denyAlienClasses )
    {
      Q_strncpyz( denied, "W", sizeof( denied ) );
    }

    dbuilder[ 0 ] = '\0';
    if( p->pers.designatedBuilder )
    {
      if( !G_admin_permission( &g_entities[ i ], ADMF_INCOGNITO ) &&
          G_admin_permission( &g_entities[ i ], ADMF_DBUILDER ) && 
          G_admin_permission(ent, ADMF_SEESFULLLISTPLAYERS ) )
      {
        Q_strncpyz( dbuilder, "P", sizeof( dbuilder ) );
      }
      else
      {
        Q_strncpyz( dbuilder, "D", sizeof( dbuilder ) );
      }
    }
    
    immune[ 0 ] = '\0';
    if( G_admin_permission( &g_entities[ i ], ADMF_BAN_IMMUNITY ) )
    {
      Q_strncpyz( immune, "I", sizeof( immune ) );
    }

    if( p->pers.paused )
    {
      // use immunity slot for paused player status
      Q_strncpyz( immune, "L", sizeof( immune ) );
    }

    l = 0;
    G_SanitiseString( p->pers.netname, n2, sizeof( n2 ) );
    n[ 0 ] = '\0';
    for( j = 0; j < MAX_ADMIN_ADMINS && g_admin_admins[ j ]; j++ )
    {
      if( !Q_stricmp( g_admin_admins[ j ]->guid, p->pers.guid ) )
      {

        // don't gather aka or level info if the admin is incognito
        if( G_admin_permission( &g_entities[ i ], ADMF_INCOGNITO ) )
        {
          break;
        }
        l = g_admin_admins[ j ]->level;
        G_SanitiseString( g_admin_admins[ j ]->name, n3, sizeof( n3 ) );
        if( Q_stricmp( n2, n3 ) )
        {
          Q_strncpyz( n, g_admin_admins[ j ]->name, sizeof( n ) );
        }
        break;
      }
    }
    lname[ 0 ] = '\0';
    Q_strncpyz( lname_fmt, "%s", sizeof( lname_fmt ) );
    for( j = 0; j < MAX_ADMIN_LEVELS && g_admin_levels[ j ]; j++ )
    {
      if( g_admin_levels[ j ]->level == l )
      {
        Q_strncpyz( lname, g_admin_levels[ j ]->name, sizeof( lname ) );
        if( *lname )
        {
          G_DecolorString( lname, lname2 );
          Com_sprintf( lname_fmt, sizeof( lname_fmt ), "%%%is",
            (int)( admin_level_maxname + strlen( lname ) - strlen( lname2 ) ) );
          Com_sprintf( lname2, sizeof( lname2 ), lname_fmt, lname );
        }
        break;
      }

    }

     
     if( G_admin_permission(ent, ADMF_SEESFULLLISTPLAYERS ) ) {
 
      ADMBP( va( "%2i %s%s^7 %-2i %s^7 (*%s) ^1%1s%1s%1s%1s^7 %s^7 %s%s^7%s\n",
               i,
               c,
               t,
               l,
               ( *lname ) ? lname2 : "", 
               guid_stub,
	       immune,
               muted,
	       dbuilder,
               denied,
               p->pers.netname,
               ( *n ) ? "(a.k.a. " : "",
               n,
               ( *n ) ? ")" : ""
             ) );
     }
     else
     {
      ADMBP( va( "%2i %s%s^7 ^1%1s%1s%1s^7 %s^7\n",
               i,
               c,
               t,
               muted,
               dbuilder,
               denied,
               p->pers.netname
             ) );
     }
  }
  ADMBP_end();
  return qtrue;
}

qboolean G_admin_listrotation( gentity_t *ent, int skiparg )
{
  int i, j, statusColor;
  char *status = '\0';
  char mapnames[ MAX_STRING_CHARS ];

  extern mapRotations_t mapRotations;

  // Check for an active map rotation
  if ( !G_MapRotationActive() ||
       g_currentMapRotation.integer == NOT_ROTATING )
  {
    trap_SendServerCommand( ent-g_entities, "print \"^3!rotation: ^7There is no active map rotation on this server\n\"" );
    return qfalse;
  }

  // Locate the active map rotation and output its contents
  for( i = 0; i < mapRotations.numRotations; i++ )
  {
    if ( i == g_currentMapRotation.integer )
    {
      int currentMap = G_GetCurrentMap( i );

      ADMBP_begin();
      ADMBP( va( "^3!rotation: ^7%s\n", mapRotations.rotations[ i ].name ) );

      for( j = 0; j < mapRotations.rotations[ i ].numMaps; j++ )
      {
        Q_strncpyz( mapnames, mapRotations.rotations[ i ].maps[ j ].name, sizeof( mapnames ) );

        if( !Q_stricmp( mapRotations.rotations[ i ].maps[ j ].name, "*VOTE*" ) )
        {
          char slotMap[ 64 ];
          int lineLen = 0;
          int k;

          trap_Cvar_VariableStringBuffer( "mapname", slotMap, sizeof( slotMap ) );
          mapnames[ 0 ] = '\0';

          for( k = 0; k < mapRotations.rotations[ i ].maps[ j ].numConditions; k++ )
          {
            char *thisMap;
            int mc = 7;

            if( mapRotations.rotations[ i ].maps[ j ].conditions[ k ].lhs != MCV_VOTE )
                continue;

            thisMap = mapRotations.rotations[ i ].maps[ j ].conditions[ k ].dest;
            lineLen += strlen( thisMap ) + 1;

            if( currentMap == j && !Q_stricmp( thisMap, slotMap ) )
              mc = 3;

            Q_strcat( mapnames, sizeof( mapnames ), va( "^7%s%s^%i%s",
              ( k ) ? ", " : "",
              ( lineLen > 50 ) ? "\n                  " : "",
              mc, thisMap ) );

            if( lineLen > 50 )
              lineLen = strlen( thisMap ) + 2;
            else
              lineLen++;
          }

          if( currentMap == j )
          {
            statusColor = 3;
            status = "current slot";
          }
          else if( !k )
          {
            statusColor = 1;
            status = "empty vote";
          }
          else
          {
            statusColor = 7;
            status = "vote";
          }
        }
        else if ( currentMap == j )
        {
          statusColor = 3;
          status = "current slot";
        }
        else if ( !G_MapExists( mapRotations.rotations[ i ].maps[ j ].name ) )
        {
          statusColor = 1;
          status = "missing";
        }
        else
        {
          statusColor = 7;
          status = "";
        }
        ADMBP( va( " ^%i%-12s %3i %s\n", statusColor, status, j + 1, mapnames ) );
      }

      ADMBP_end();

      // No maps were found in the active map rotation
      if ( mapRotations.rotations[ i ].numMaps < 1 )
      {
        trap_SendServerCommand( ent-g_entities, "print \"  - ^7Empty!\n\"" );
        return qfalse;
      }
    }
  }

  if( g_nextMap.string[0] )
      ADMP( va ("^5 Next map overriden to: %s\n", g_nextMap.string ) );

  return qtrue;
}

qboolean G_admin_showbans( gentity_t *ent, int skiparg )
{
  int i, found = 0;
  qtime_t qt;
  int t;
  char duration[ 32 ];
  char sduration[ 32 ];
  char suspended[ 64 ] = { "" };
  char name_fmt[ 32 ] = { "%s" };
  char banner_fmt[ 32 ] = { "%s" };
  int max_name = 1, max_banner = 1;
  int secs;
  int start = 0;
  char skip[ MAX_NAME_LENGTH ];
  char date[ 11 ];
  char *made;
  int j;
  char n1[ MAX_NAME_LENGTH ] = {""};
  char n2[ MAX_NAME_LENGTH ] = {""};
  qboolean numeric = qtrue;
  char *ip_match = NULL;
  int ip_match_len = 0;
  char name_match[ MAX_NAME_LENGTH ] = {""};
  int show_count = 0;

  t = trap_RealTime( &qt );

  for( i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++ )
  {
    if( g_admin_bans[ i ]->expires != 0
      && ( g_admin_bans[ i ]->expires - t ) < 1 )
    {
      continue;
    }
    found++;
  }

  if( G_SayArgc() < 3 + skiparg )
  {
    G_SayArgv( 1 + skiparg, skip, sizeof( skip ) );
    for( i = 0; i < sizeof( skip ) && skip[ i ] ; i++ )
    {
      if( ( skip[ i ] < '0' || skip[ i ] > '9' )
        && skip[ i ] != '.' && skip[ i ] != '-' )
      {
        numeric = qfalse;
        break;
      }
    }

    if (!numeric)
      {
      G_SanitiseString( skip, name_match, sizeof( name_match ) );
      }
    else if( strchr( skip, '.' ) != NULL )
      {
      ip_match = skip;
      ip_match_len = strlen(ip_match);
      }
    else
      {
      start = atoi( skip );
      }
    // showbans 1 means start with ban 0
    if( start > 0 )
      start -= 1;
    else if( start < 0 )
      start = found + start;
  }

  if( start >= MAX_ADMIN_BANS || start < 0 )
    start = 0;

  for( i = start; i < MAX_ADMIN_BANS && g_admin_bans[ i ] 
    && show_count < MAX_ADMIN_SHOWBANS; i++ )
  {
    qboolean match = qfalse;

    if (!numeric)
      {
      G_SanitiseString( g_admin_bans[ i ]->name, n1, sizeof( n1 ) );
      if (strstr( n1, name_match) )
        match = qtrue;
      }

    if ( ( match ) || !ip_match
      || Q_strncmp( ip_match, g_admin_bans[ i ]->ip, ip_match_len) == 0 )
    {
      G_DecolorString( g_admin_bans[ i ]->name, n1 );
      G_DecolorString( g_admin_bans[ i ]->banner, n2 );
      if( strlen( n1 ) > max_name )
      {
        max_name = strlen( n1 );
      }
      if( strlen( n2 ) > max_banner )
        max_banner = strlen( n2 );
 
      show_count++;
    }
  }

  if( start >= found )
  {
    ADMP( va( "^3!showbans: ^7there are %d active bans\n", found ) );
    return qfalse;
  }
  ADMBP_begin();
  show_count = 0;
  for( i = start; i < MAX_ADMIN_BANS && g_admin_bans[ i ]
    && show_count < MAX_ADMIN_SHOWBANS; i++ )
  {
    if( g_admin_bans[ i ]->expires != 0
      && ( g_admin_bans[ i ]->expires - t ) < 1 )
      continue;

    if (!numeric)
    {
      G_SanitiseString( g_admin_bans[ i ]->name, n1, sizeof( n1 ) );
      if ( strstr ( n1, name_match ) == NULL )
        continue;
    }
    else if( ip_match != NULL
      && Q_strncmp( ip_match, g_admin_bans[ i ]->ip, ip_match_len ) != 0)
      continue;

    // only print out the the date part of made
    date[ 0 ] = '\0';
    made = g_admin_bans[ i ]->made;
    for( j = 0; made && *made; j++ )
    {
      if( ( j + 1 ) >= sizeof( date ) )
        break;
      if( *made == ' ' )
        break;
      date[ j ] = *made;
      date[ j + 1 ] = '\0';
      made++;
    }

    secs = ( g_admin_bans[ i ]->expires - t );
    G_admin_duration( secs, duration, sizeof( duration ) );
    
    if( g_admin_bans[ i ]->suspend > t )
    {
      G_admin_duration( g_admin_bans[ i ]->suspend - t, sduration, sizeof( sduration ) );
      Com_sprintf( suspended, sizeof( suspended ), "     | - SUSPENDED for %s\n",
        sduration );
    }
    else
    {
      suspended[ 0 ] = '\0';
    }

    G_DecolorString( g_admin_bans[ i ]->name, n1 );
    Com_sprintf( name_fmt, sizeof( name_fmt ), "%%%is",
      (int)( max_name + strlen( g_admin_bans[ i ]->name ) - strlen( n1 ) ) );
    Com_sprintf( n1, sizeof( n1 ), name_fmt, g_admin_bans[ i ]->name ); 

    G_DecolorString( g_admin_bans[ i ]->banner, n2 );
    Com_sprintf( banner_fmt, sizeof( banner_fmt ), "%%%is",
      (int)( max_banner + strlen( g_admin_bans[ i ]->banner ) - strlen( n2 ) ) );
    Com_sprintf( n2, sizeof( n2 ), banner_fmt, g_admin_bans[ i ]->banner ); 

    ADMBP( va( "%4i %s^7 %-15s %-8s %s^7 %-10s\n%s     \\__ %s\n",
             ( i + 1 ),
             n1,
             g_admin_bans[ i ]->ip,
             date,
             n2,
             duration,
             suspended,
             g_admin_bans[ i ]->reason ) );

    show_count++;
  }

  if (!numeric || ip_match)
  {
    ADMBP( va( "^3!showbans:^7 found %d matching bans by %s.  ",
             show_count,
             (numeric) ? "IP" : "name" ) );
  }
  else
  {
    ADMBP( va( "^3!showbans:^7 showing bans %d - %d of %d.  ",
             ( found ) ? ( start + 1 ) : 0,
             ( ( start + MAX_ADMIN_SHOWBANS ) > found ) ?
             found : ( start + MAX_ADMIN_SHOWBANS ),
             found ) );
    if( ( start + MAX_ADMIN_SHOWBANS ) < found )
    {
      ADMBP( va( "run !showbans %d to see more",
               ( start + MAX_ADMIN_SHOWBANS + 1 ) ) );
    }
  }
  ADMBP( "\n" );
  ADMBP_end();
  return qtrue;
}

qboolean G_admin_help( gentity_t *ent, int skiparg )
{
  int i;

  if( G_SayArgc() < 2 + skiparg )
  {
    int j = 0;
    int count = 0;

    ADMBP_begin();
    for( i = 0; i < adminNumCmds; i++ )
    {
      if( G_admin_permission( ent, g_admin_cmds[ i ].flag ) )
      {
        ADMBP( va( "^3!%-12s", g_admin_cmds[ i ].keyword ) );
        j++;
        count++;
      }
      // show 6 commands per line
      if( j == 6 )
      {
	ADMBP( "\n" );
        j = 0;
      }
    }
    for( i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++ )
    {
      if( !G_admin_permission( ent, g_admin_commands[ i ]->flag ) )
        continue;
      ADMBP( va( "^3!%-12s", g_admin_commands[ i ]->command ) );
      j++;
      count++;
      // show 6 commands per line
      if( j == 6 )
      {
	ADMBP( "\n" );
        j = 0;
      }
    }
    if( count )
	ADMBP( "\n" );
    ADMBP( va( "^3!help: ^7%i available commands\n", count ) );
    ADMBP( "run !help [^3command^7] for help with a specific command.\n" );
    ADMBP_end();

    return qtrue;
  }
  else
  {
    //!help param
    char param[ MAX_ADMIN_CMD_LEN ];
    char *cmd;

    G_SayArgv( 1 + skiparg, param, sizeof( param ) );
    cmd = ( param[0] == '!' ) ? &param[1] : &param[0];
    ADMBP_begin();
    for( i = 0; i < adminNumCmds; i++ )
    {
      if( !Q_stricmp( cmd, g_admin_cmds[ i ].keyword ) )
      {
        if( !G_admin_permission( ent, g_admin_cmds[ i ].flag ) )
        {
          ADMBP( va( "^3!help: ^7you have no permission to use '%s'\n",
                   g_admin_cmds[ i ].keyword ) );
          ADMBP_end();
          return qfalse;
        }
        ADMBP( va( "^3!help: ^7help for '!%s':\n",
          g_admin_cmds[ i ].keyword ) );
        ADMBP( va( " ^3Function: ^7%s\n", g_admin_cmds[ i ].function ) );
        ADMBP( va( " ^3Syntax: ^7!%s %s\n", g_admin_cmds[ i ].keyword,
                 g_admin_cmds[ i ].syntax ) );
        ADMBP( va( " ^3Flag: ^7'%s'\n", g_admin_cmds[ i ].flag ) );
        ADMBP_end();
        return qtrue;
      }
    }
    for( i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++ )
    {
      if( !Q_stricmp( cmd, g_admin_commands[ i ]->command ) )
      {
        if( !G_admin_permission( ent, g_admin_cmds[ i ].flag ) )
        {
          ADMBP( va( "^3!help: ^7you have no permission to use '%s'\n",
                   g_admin_commands[ i ]->command ) );
          ADMBP_end();
          return qfalse;
        }
        ADMBP( va( "^3!help: ^7help for '%s':\n",
          g_admin_commands[ i ]->command ) );
        ADMBP( va( " ^3Description: ^7%s\n", g_admin_commands[ i ]->desc ) );
        ADMBP( va( " ^3Syntax: ^7!%s\n", g_admin_commands[ i ]->command ) );
        ADMBP_end();
        return qtrue;
      }
    }
    ADMBP( va( "^3!help: ^7no help found for '%s'\n", cmd ) );
    ADMBP_end();
    return qfalse;
  }
}

void G_Unescape( char *input, char *output, int len );
qboolean G_StringReplaceCvars( char *input, char *output, int len );

qboolean G_admin_info( gentity_t *ent, int skiparg )
{
  fileHandle_t infoFile;
  int length;
  char filename[ MAX_OSPATH ], message[ MAX_STRING_CHARS ];
  if( G_SayArgc() == 2 + skiparg )
    G_SayArgv( 1 + skiparg, filename, sizeof( filename ) );
  else if( G_SayArgc() == 1 + skiparg )
    Q_strncpyz( filename, "default", sizeof( filename ) );
  else
  {
    ADMP( "^3!info: ^7usage: ^3!info ^7(^5subject^7)\n" );
    return qfalse;
  }
  Com_sprintf( filename, sizeof( filename ), "info/info-%s.txt", filename );
  length = trap_FS_FOpenFile( filename, &infoFile, FS_READ );
  if( length <= 0 || !infoFile )
  {
    trap_FS_FCloseFile( infoFile );
    ADMP( "^3!info: ^7no relevant information is available\n" );
    return qfalse;
  }
  else
  {
    int i;
    char *cr;
    trap_FS_Read( message, sizeof( message ), infoFile );
    if( length < sizeof( message ) )
      message[ length ] = '\0';
    else
      message[ sizeof( message ) - 1 ] = '\0';
    trap_FS_FCloseFile( infoFile );
    // strip carriage returns for windows platforms
    while( ( cr = strchr( message, '\r' ) ) )
      memmove( cr, cr + 1, strlen( cr + 1 ) + 1 );
#define MAX_INFO_PARSE_LOOPS 100
    for( i = 0; i < MAX_INFO_PARSE_LOOPS &&
        G_StringReplaceCvars( message, message, sizeof( message ) ); i++ );
    G_Unescape( message, message, sizeof( message ) );
    if( i == MAX_INFO_PARSE_LOOPS )
      G_Printf( S_COLOR_YELLOW "WARNING: %s exceeds MAX_INFO_PARSE_LOOPS\n", filename );
    ADMP( va( "%s\n", message ) );
    return qtrue;
  }
}

void G_Unescape( char *input, char *output, int len )
{
  // \n -> newline, \%c -> %c
  // output is terminated at output[len - 1]
  // it's OK for input to equal output, because our position in input is always
  // equal or greater than our position in output
  // however, if output is later in the same string as input, a crash is pretty
  // much inevitable
  int i, j;
  for( i = j = 0; input[i] && j + 1 < len; i++, j++ )
  {
    if( input[i] == '\\' )
    {
      if( !input[++i] )
      {
        output[j] = '\0';
        return;
      }
      else if( input[i] == 'n' )
        output[j] = '\n';
      else
        output[j] = input[i];
    }
    else
      output[j] = input[i];
  }
  output[j] = '\0';
}

qboolean G_StringReplaceCvars( char *input, char *output, int len )
{
  int i, outNum = 0;
  char cvarName[ 64 ], cvarValue[ MAX_CVAR_VALUE_STRING ];
  char *outputBuffer;
  qboolean doneAnything = qfalse;
  if( len <= 0 )
    return qfalse;
  // use our own internal buffer in case output == input
  outputBuffer = G_Alloc( len );
  len -= 1; // fit in a terminator
  while( *input && outNum < len )
  {
    if( *input == '\\' && input[1] && outNum < len - 1 )
    {
      outputBuffer[ outNum++ ] = *input++;
      outputBuffer[ outNum++ ] = *input++;
    }
    else if( *input == '$' )
    {
      doneAnything = qtrue;
      input++;
      if( *input == '{' )
        input++;
      for( i = 0; *input && ( isalnum( *input ) || *input == '_' ) &&
          i < 63; i++ )
        cvarName[ i ] = *input++;
      cvarName[ i ] = '\0';
      if( *input == '}' )
        input++;
      trap_Cvar_VariableStringBuffer( cvarName, cvarValue, sizeof( cvarValue ) );
      if( cvarValue[ 0 ] )
      {
        for( i = 0; cvarValue[ i ] && outNum < len; i++ )
          outputBuffer[ outNum++ ] = cvarValue[ i ];
      }
    }
    else
      outputBuffer[ outNum++ ] = *input++;
  }
  outputBuffer[ outNum ] = '\0';
  Q_strncpyz( output, outputBuffer, len );
  G_Free( outputBuffer );
  return doneAnything;
}

qboolean G_admin_admintest( gentity_t *ent, int skiparg )
{
  int i, l = 0;
  qboolean found = qfalse;
  qboolean lname = qfalse;

  if( !ent )
  {
    ADMP( "^3!admintest: ^7you are on the console.\n" );
    return qtrue;
  }
  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    if( !Q_stricmp( g_admin_admins[ i ]->guid, ent->client->pers.guid ) )
    {
      found = qtrue;
      break;
    }
  }

  if( found )
  {
    l = g_admin_admins[ i ]->level;
    for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
    {
      if( g_admin_levels[ i ]->level != l )
        continue;
      if( *g_admin_levels[ i ]->name )
      {
        lname = qtrue;
        break;
      }
    }
  }
  AP( va( "print \"^3!admintest: ^7%s^7 is a level %d admin %s%s^7%s\n\"",
          ent->client->pers.netname,
          l,
          ( lname ) ? "(" : "",
          ( lname ) ? g_admin_levels[ i ]->name : "",
          ( lname ) ? ")" : "" ) );
  return qtrue;
}

qboolean G_admin_allready( gentity_t *ent, int skiparg )
{
  int i = 0;
  gclient_t *cl;

  if( !level.intermissiontime )
  {
    ADMP( "^3!allready: ^7this command is only valid during intermission\n" );
    return qfalse;
  }

  for( i = 0; i < g_maxclients.integer; i++ )
  {
    cl = level.clients + i;
    if( cl->pers.connected != CON_CONNECTED )
      continue;

    if( cl->pers.teamSelection == PTE_NONE )
      continue;

    cl->readyToExit = 1;
  }
  AP( va( "print \"^3!allready:^7 %s^7 says everyone is READY now\n\"",
     ( ent ) ? ent->client->pers.netname : "console" ) ); 
  return qtrue;
}

qboolean G_admin_cancelvote( gentity_t *ent, int skiparg )
{

  if(!level.voteTime && !level.teamVoteTime[ 0 ] && !level.teamVoteTime[ 1 ] )
  {
    ADMP( "^3!cancelvote^7: no vote in progress\n" );
    return qfalse;
  }
  if( !Q_strncmp( level.voteDisplayString, "Extend", 6 )
    && level.extend_vote_count > 0 )
  {
    level.extend_vote_count--;
  }
  level.voteNo = level.numConnectedClients;
  level.voteYes = 0;
  CheckVote( );
  level.teamVoteNo[ 0 ] = level.numConnectedClients;
  level.teamVoteYes[ 0 ] = 0;
  CheckTeamVote( PTE_HUMANS );
  level.teamVoteNo[ 1 ] = level.numConnectedClients;
  level.teamVoteYes[ 1 ] = 0;
  CheckTeamVote( PTE_ALIENS );
  AP( va( "print \"^3!cancelvote: ^7%s^7 decided that everyone voted No\n\"",
          ( ent ) ? ent->client->pers.netname : "console" ) );
  return qtrue;
}

qboolean G_admin_passvote( gentity_t *ent, int skiparg )
{
  if(!level.voteTime && !level.teamVoteTime[ 0 ] && !level.teamVoteTime[ 1 ] )
  {
    ADMP( "^3!passvote^7: no vote in progress\n" );
    return qfalse;
  }
  level.voteYes = level.numConnectedClients;
  level.voteNo = 0;
  CheckVote( );
  level.teamVoteYes[ 0 ] = level.numConnectedClients;
  level.teamVoteNo[ 0 ] = 0;
  CheckTeamVote( PTE_HUMANS );
  level.teamVoteYes[ 1 ] = level.numConnectedClients;
  level.teamVoteNo[ 1 ] = 0;
  CheckTeamVote( PTE_ALIENS );
  AP( va( "print \"^3!passvote: ^7%s^7 decided that everyone voted Yes\n\"",
          ( ent ) ? ent->client->pers.netname : "console" ) );
  return qtrue;
}

qboolean G_admin_spec999( gentity_t *ent, int skiparg )
{
  int i;
  gentity_t *vic;

  for( i = 0; i < level.maxclients; i++ )
  {
    vic = &g_entities[ i ];
    if( !vic->client )
      continue;
    if( vic->client->pers.connected != CON_CONNECTED )
      continue;
    if( vic->client->pers.teamSelection == PTE_NONE )
      continue;
    if( vic->client->ps.ping == 999 )
    {
      G_ChangeTeam( vic, PTE_NONE );
      AP( va( "print \"^3!spec999: ^7%s^7 moved ^7%s^7 to spectators\n\"",
        ( ent ) ? ent->client->pers.netname : "console", 
        vic->client->pers.netname ) );
    }
  }
  return qtrue;
}

qboolean G_admin_register(gentity_t *ent, int skiparg ){
  int level = 0;
  int max = 0;
  char buffer [ 64 ];

  if( !ent )
  {
    ADMP( "!register: sorry, but console isn't allowed to register.\n");
    return qfalse;
  }

  level = G_admin_level(ent);

  if( G_SayArgc() > 1 + skiparg )
  {
    G_SayArgv( 1 + skiparg, buffer, sizeof( buffer ) );
    max = atoi( buffer );

    if( G_SayArgc() > 2 + skiparg )
    {
      G_SayArgv( 2 + skiparg, buffer, sizeof( buffer ) );
      if( g_adminRegisterAdminPass.string[ 0 ] && Q_stricmp( g_adminRegisterAdminPass.string, "none" ) &&
        Q_stricmp( g_adminRegisterAdminPass.string, buffer) )
      {
        ADMP( "Invalid password.\n" );
        return qfalse;
      }
      if (max > g_adminRegisterAdminLevel.integer)
        max = g_adminRegisterAdminLevel.integer;
      if( level <= g_adminRegisterAdminLevel.integer )
        level = max;
    }
    else
    {
      if( max > g_adminRegisterLevel.integer )
        max = g_adminRegisterLevel.integer;

      if( level > 0 &&
        level <= g_adminRegisterLevel.integer )
      {
        level = max;
      }
    }

    if( max < -1 )
      max = -1;
  }
  else
  {
    if( max == 0 )
      max = 1;
  }

  if( level < max )
    level = max;
  
  if( !Q_stricmp( ent->client->pers.guid, "XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX" ) )
  {
    ADMP( va( "^3!register: ^7 You cannot register for name protection until you update your client. Please replace your client executable with the one at http://www.tremulous.net.ar/descargas and reconnect. Updating your client will also allow you to have faster map downloads.\n" ) );
    ADMP( va( "^3!register: ^7 No puedes registrarte para proteger tu nombre hasta que no hayas actualizado tu cliente. Por favor, descarga e instala el que se encuentra en http://www.tremulous.net.ar/descargas y vuelve a conectarte. Actualizar tu cliente tambien te permitira descargar mapas mucho mas rapido.\n" ) );
    return qfalse;
  }
  
  if( g_newbieNumbering.integer
    && g_newbieNamePrefix.string[ 0 ]
    && Q_stricmpn ( ent->client->pers.netname, g_newbieNamePrefix.string, strlen(g_newbieNamePrefix.string ) ) == 0 )
  {
    ADMP( va( "^3!register: ^7 You cannot register names that begin with '%s^7'.\n",
      g_newbieNamePrefix.string ) );
    return qfalse;
  }

  trap_SendConsoleCommand( EXEC_APPEND, va( "!setlevel %i %d;", (int)(ent - g_entities), level) );
  ClientUserinfoChanged( ent - g_entities );
  
  AP( va( "print \"^3!register: ^7%s^7 is now a protected nickname.\n\"", ent->client->pers.netname) );
  
  return qtrue;
}

qboolean G_admin_rename( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ];
  char newname[ MAX_NAME_LENGTH ];
  char oldname[ MAX_NAME_LENGTH ];
  char err[ MAX_STRING_CHARS ];
  char userinfo[ MAX_INFO_STRING ];
  char *s;
  gentity_t *victim = NULL;

  if( G_SayArgc() < 3 + skiparg )
  {
    ADMP( "^3!rename: ^7usage: !rename [name] [newname]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  s = G_SayConcatArgs( 2 + skiparg );
  Q_strncpyz( newname, s, sizeof( newname ) );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!rename: ^7%s\n", err ) );
    return qfalse;
  }
  victim = &g_entities[ pids[ 0 ] ] ;
  if( !admin_higher( ent, victim ) )
  {
    ADMP( "^3!rename: ^7sorry, but your intended victim has a higher admin"
        " level than you\n" );
    return qfalse;
  }
  if( !G_admin_name_check( victim, newname, err, sizeof( err ) ) )
  {
    ADMP( va( "^3!rename: ^7%s\n", err ) );
    return qfalse;
  }
  level.clients[ pids[ 0 ] ].pers.nameChanges--;
  level.clients[ pids[ 0 ] ].pers.nameChangeTime = 0;
  trap_GetUserinfo( pids[ 0 ], userinfo, sizeof( userinfo ) );
  s = Info_ValueForKey( userinfo, "name" );
  Q_strncpyz( oldname, s, sizeof( oldname ) );
  Info_SetValueForKey( userinfo, "name", newname );
  trap_SetUserinfo( pids[ 0 ], userinfo );
  ClientUserinfoChanged( pids[ 0 ] );
  if( strcmp( oldname, level.clients[ pids[ 0 ] ].pers.netname ) )
    AP( va( "print \"^3!rename: ^7%s^7 has been renamed to %s^7 by %s\n\"",
	    oldname,
	    level.clients[ pids[ 0 ] ].pers.netname,
	    ( ent ) ? ent->client->pers.netname : "console" ) );
  return qtrue;
}

qboolean G_admin_restart( gentity_t *ent, int skiparg )
{
  char layout[ MAX_CVAR_VALUE_STRING ] = { "" };
  char teampref[ MAX_CVAR_VALUE_STRING ] = { "" };
  int i;
  gclient_t *cl;

  if( G_SayArgc( ) > 1 + skiparg )
  {
    char map[ MAX_QPATH ];

    trap_Cvar_VariableStringBuffer( "mapname", map, sizeof( map ) );
    G_SayArgv( skiparg + 1, layout, sizeof( layout ) );

    if( Q_stricmp( layout, "keepteams" ) && Q_stricmp( layout, "keepteamslock" ) && Q_stricmp( layout, "switchteams" ) && Q_stricmp( layout, "switchteamslock" ) )
    {
      if( !Q_stricmp( layout, "*BUILTIN*" ) ||
        trap_FS_FOpenFile( va( "layouts/%s/%s.dat", map, layout ),
          NULL, FS_READ ) > 0 )
      {
        trap_Cvar_Set( "g_layouts", layout );
      }
      else
      {
        ADMP( va( "^3!restart: ^7layout '%s' does not exist\n", layout ) );
        return qfalse;
      }
    }
    else 
    {
      strcpy(layout,"");
      G_SayArgv( skiparg + 1, teampref, sizeof( teampref ) );    
    }
  }
  
  if( G_SayArgc( ) > 2 + skiparg ) 
  {
    G_SayArgv( skiparg + 2, teampref, sizeof( teampref ) );      
  }
  
  
  if( !Q_stricmp( teampref, "keepteams" ) || !Q_stricmp( teampref, "keepteamslock" ) )
  {
    for( i = 0; i < g_maxclients.integer; i++ )
    {
      cl = level.clients + i;
      if( cl->pers.connected != CON_CONNECTED )
        continue;

      if( cl->pers.teamSelection == PTE_NONE )
        continue;

      cl->sess.restartTeam = cl->pers.teamSelection;
    }
  } 
  else if(!Q_stricmp( teampref, "switchteams" ) ||  !Q_stricmp( teampref, "switchteamslock" ))
  {
    for( i = 0; i < g_maxclients.integer; i++ )
    {
      cl = level.clients + i;
      if( cl->pers.connected != CON_CONNECTED )
        continue;

      if( cl->pers.teamSelection == PTE_NONE )
        continue;

      if( cl->pers.teamSelection == PTE_ALIENS )
        cl->sess.restartTeam = PTE_HUMANS;
      else if(cl->pers.teamSelection == PTE_HUMANS )
	cl->sess.restartTeam = PTE_ALIENS;
    }  	  
  }
  
  if( !Q_stricmp( teampref, "switchteamslock" ) || !Q_stricmp( teampref, "keepteamslock" ) )
  {
    trap_Cvar_Set( "g_lockTeamsAtStart", "1" );
  }

  G_admin_maplog_result( "R" );

  trap_SendConsoleCommand( EXEC_APPEND, "map_restart" );
  
  if(teampref[ 0 ])
    strcpy(teampref,va( "^7(with teams option: '%s^7')", teampref ));
  
  AP( va( "print \"^3!restart: ^7map restarted by %s %s %s\n\"",
          ( ent ) ? ent->client->pers.netname : "console",
          ( layout[ 0 ] ) ? va( "^7(forcing layout '%s^7')", layout ) : "",
          teampref ) );

  if( level.scrimmMode )
  {
      G_admin_scrimm_switch( qfalse );
      trap_Cvar_Set( "g_scrimmRestarted", "1" );
      AP( "print \"^3Scrimm ^7mode is ^2ON\n\"" );
  }

  return qtrue;
}

qboolean G_admin_nextmap( gentity_t *ent, int skiparg )
{
  AP( va( "print \"^3!nextmap: ^7%s^7 decided to load the next map\n\"",
    ( ent ) ? ent->client->pers.netname : "console" ) );
  level.lastWin = PTE_NONE;
  trap_SetConfigstring( CS_WINNER, "Evacuation" );
  LogExit( va( "nextmap was run by %s",
    ( ent ) ? ent->client->pers.netname : "console" ) );
  G_admin_maplog_result( "N" );
  return qtrue;
}

qboolean G_admin_namelog( gentity_t *ent, int skiparg )
{
  int i, j;
  char search[ MAX_NAME_LENGTH ] = {""};
  char s2[ MAX_NAME_LENGTH ] = {""};
  char n2[ MAX_NAME_LENGTH ] = {""};
  char guid_stub[ 9 ];
  qboolean found = qfalse;
  int printed = 0;

  if( G_SayArgc() > 1 + skiparg )
  {
    G_SayArgv( 1 + skiparg, search, sizeof( search ) );
    G_SanitiseString( search, s2, sizeof( s2 ) );
  }
  ADMBP_begin();
  for( i = 0; i < MAX_ADMIN_NAMELOGS && g_admin_namelog[ i ]; i++ )
  {
    if( search[0] )
    {
      found = qfalse;
      for( j = 0; j < MAX_ADMIN_NAMELOG_NAMES && 
        g_admin_namelog[ i ]->name[ j ][ 0 ]; j++ )
      {
        G_SanitiseString( g_admin_namelog[ i ]->name[ j ], n2, sizeof( n2 ) );
        if( strstr( n2, s2 ) )
        {
          found = qtrue;
          break;
        }
      }
      if( !found )
        continue;
    }
    printed++;
    for( j = 0; j < 8; j++ )
      guid_stub[ j ] = g_admin_namelog[ i ]->guid[ j + 24 ];
    guid_stub[ j ] = '\0';
    if( g_admin_namelog[ i ]->slot > -1 )
       ADMBP( "^3" );
    ADMBP( va( "%-2s (*%s) %15s ^5%s^7",
      (g_admin_namelog[ i ]->slot > -1 ) ?
        va( "%d", g_admin_namelog[ i ]->slot ) : "-",
      guid_stub, g_admin_namelog[ i ]->ip,
      g_admin_namelog[ i ]->country ) );
    for( j = 0; j < MAX_ADMIN_NAMELOG_NAMES && 
      g_admin_namelog[ i ]->name[ j ][ 0 ]; j++ )
    {
      ADMBP( va( " '%s^7'", g_admin_namelog[ i ]->name[ j ] ) );
    }
    ADMBP( "\n" ); 
  } 
  ADMBP( va( "^3!namelog:^7 %d recent clients found\n", printed ) );
  ADMBP_end();
  
  return qtrue;
}

qboolean G_admin_lock( gentity_t *ent, int skiparg )
{
  char teamName[2] = {""};
  pTeam_t team;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!lock: ^7usage: !lock [a|h]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, teamName, sizeof( teamName ) );
  if( teamName[ 0 ] == 'a' || teamName[ 0 ] == 'A' )
    team = PTE_ALIENS;
  else if( teamName[ 0 ] == 'h' || teamName[ 0 ] == 'H' )
    team = PTE_HUMANS;
  else
  {
    ADMP( va( "^3!lock: ^7invalid team\"%c\"\n", teamName[0] ) );
    return qfalse;
  }

  if( team == PTE_ALIENS )
  {
    if( level.alienTeamLocked )
    {
      ADMP( "^3!lock: ^7Alien team is already locked\n" );
      return qfalse;
    }
    else
      level.alienTeamLocked = qtrue;
  }
  else if( team == PTE_HUMANS ) {
    if( level.humanTeamLocked )
    {
      ADMP( "^3!lock: ^7Human team is already locked\n" );
      return qfalse;
    }
    else
      level.humanTeamLocked = qtrue;
  }

  AP( va( "print \"^3!lock: ^7%s team has been locked by %s\n\"",
    ( team == PTE_ALIENS ) ? "Alien" : "Human",
    ( ent ) ? ent->client->pers.netname : "console" ) );
  return qtrue;
} 

qboolean G_admin_unlock( gentity_t *ent, int skiparg )
{
  char teamName[2] = {""};
  pTeam_t team;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!unlock: ^7usage: !unlock [a|h]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, teamName, sizeof( teamName ) );
  if( teamName[ 0 ] == 'a' || teamName[ 0 ] == 'A' )
    team = PTE_ALIENS;
  else if( teamName[ 0 ] == 'h' || teamName[ 0 ] == 'H' )
    team = PTE_HUMANS;
  else
  {
    ADMP( va( "^3!unlock: ^7invalid team\"%c\"\n", teamName[0] ) );
    return qfalse;
  }
  
  if( team == PTE_ALIENS )
  {
    if( !level.alienTeamLocked )
    {
      ADMP( "^3!unlock: ^7Alien team is not currently locked\n" );
      return qfalse;
    }
    else
      level.alienTeamLocked = qfalse;
  }
  else if( team == PTE_HUMANS ) {
    if( !level.humanTeamLocked )
    {
      ADMP( "^3!unlock: ^7Human team is not currently locked\n" );
      return qfalse;
    }
    else
      level.humanTeamLocked = qfalse;
  }

  AP( va( "print \"^3!unlock: ^7%s team has been unlocked by %s\n\"",
    ( team == PTE_ALIENS ) ? "Alien" : "Human",
    ( ent ) ? ent->client->pers.netname : "console" ) );
  return qtrue;
} 

qboolean G_admin_designate( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  char command[ MAX_ADMIN_CMD_LEN ], *cmd;
  gentity_t *vic;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!designate: ^7usage: designate [name|slot#]\n" );
    return qfalse;
  }
  G_SayArgv( skiparg, command, sizeof( command ) );
  cmd = command;
  if( cmd && *cmd == '!' )
    cmd++;
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!designate: ^7%s\n", err ) );
    return qfalse;
  }
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) &&
    !Q_stricmp( cmd, "undesignate" ) )
  {
    ADMP( "^3!mute: ^7sorry, but your intended victim has a higher admin"
        " level than you\n" );
    return qfalse;
  }
  vic = &g_entities[ pids[ 0 ] ];
  if( vic->client->pers.designatedBuilder == qtrue )
  {
    if( !Q_stricmp( cmd, "designate" ) )
    {
      ADMP( "^3!designate: ^7player is already designated builder\n" );
      return qtrue;
    }
    vic->client->pers.designatedBuilder = qfalse;
    CPx( pids[ 0 ], "cp \"^1Your designation has been revoked\"" );
    AP( va(
      "print \"^3!designate: ^7%s^7's designation has been revoked by %s\n\"",
       vic->client->pers.netname,
       ( ent ) ? ent->client->pers.netname : "console" ) );
    G_CheckDBProtection( );
  }
  else
  {
    if( !Q_stricmp( cmd, "undesignate" ) )
    {
      ADMP( "^3!undesignate: ^7player is not currently designated builder\n" );
      return qtrue;
    }
    vic->client->pers.designatedBuilder = qtrue;
    CPx( pids[ 0 ], "cp \"^1You've been designated\"" );
    AP( va( "print \"^3!designate: ^7%s^7 has been designated by ^7%s\n\"",
      vic->client->pers.netname,
      ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  ClientUserinfoChanged( pids[ 0 ] );
  return qtrue;
}

 qboolean G_admin_warn( gentity_t *ent, int skiparg )
 {
   int pids[ MAX_CLIENTS ];
   char name[ MAX_NAME_LENGTH ], *reason, err[ MAX_STRING_CHARS ];
   int minargc;
   gentity_t *vic;
 
   minargc = 3 + skiparg;
   if( G_admin_permission( ent, ADMF_UNACCOUNTABLE ) )
     minargc = 2 + skiparg;
 
   if( G_SayArgc() < minargc )
   {
     ADMP( "^3!warn: ^7usage: warn [name] [reason]\n" );
     return qfalse;
   }
   G_SayArgv( 1 + skiparg, name, sizeof( name ) );
   reason = G_SayConcatArgs( 2 + skiparg );
   if( G_ClientNumbersFromString( name, pids ) != 1 )
   {
     G_MatchOnePlayer( pids, err, sizeof( err ) );
     ADMP( va( "^3!warn: ^7%s\n", err ) );
     return qfalse;
   }
   if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
   {
     ADMP( "^3!warn: ^7sorry, but your intended victim has a higher admin"
         " level than you.\n" );
     return qfalse;
   }
  
   vic = &g_entities[ pids[ 0 ] ];
   //next line is the onscreen warning
   CPx( pids[ 0 ],va("cp \"^1You have been warned by an administrator.\n ^3Cease imeediately or face admin action!\n^1 %s%s\"",(*reason)? "REASON: " : "" ,(*reason)? reason : "") );
   AP( va( "print \"^3!warn: ^7%s^7 has been warned to cease and desist %s by %s \n\"",
             vic->client->pers.netname, (*reason) ? reason : "his current activity",
             ( ent ) ? ent->client->pers.netname : "console" ) );//console announcement
   return qtrue;
 }

qboolean G_admin_putmespec( gentity_t *ent, int skiparg )
{
  if( !ent )
  {
	ADMP( "!specme: sorry, but console isn't allowed on the spectators team\n");
	return qfalse;
  }

  if( level.paused )
  {
    ADMP("!specme: disabled when game is paused\n");
    return qfalse;
  }
  
  if(ent->client->pers.teamSelection == PTE_NONE)
    return qfalse;
  
    //guard against build timer exploit
  if( ent->client->pers.teamSelection != PTE_NONE && ent->client->sess.sessionTeam != TEAM_SPECTATOR &&
     ( ent->client->ps.stats[ STAT_PCLASS ] == PCL_ALIEN_BUILDER0 ||
       ent->client->ps.stats[ STAT_PCLASS ] == PCL_ALIEN_BUILDER0_UPG ||
       BG_InventoryContainsWeapon( WP_HBUILD, ent->client->ps.stats ) ||
       BG_InventoryContainsWeapon( WP_HBUILD2, ent->client->ps.stats ) ) &&
      ent->client->ps.stats[ STAT_MISC ] > 0 )
  {
    ADMP("!specme: You cannot leave your team until the build timer expires");
    return qfalse;
  }
  
  G_ChangeTeam( ent, PTE_NONE );

  // check for silent '!specme s' - requires !kick permission
  if( G_SayArgc() > 1 + skiparg )
  {
    char arg[ 2 ];

    G_SayArgv( 1 + skiparg, arg, sizeof( arg ) );
    if( ( arg[0] == 's' || arg[0] == 'S' ) && G_admin_permission( ent, "kick" ) )
    {
      ADMP("^3!specme: ^7 You have silently joined the spectators\n");
      return qtrue;
    }
  }

  AP( va("print \"^3!specme: ^7%s^7 decided to join the spectators\n\"", ent->client->pers.netname ) );
  return qtrue;
}

/*
================
 G_admin_print

 This function facilitates the ADMP define.  ADMP() is similar to CP except
 that it prints the message to the server console if ent is not defined.
================
*/
void G_admin_print( gentity_t *ent, char *m )
{
  if( ent )
    trap_SendServerCommand( ent - level.gentities, va( "print \"%s\"", m ) );
  else
  {
    char m2[ MAX_STRING_CHARS ];
    if( !trap_Cvar_VariableIntegerValue( "com_ansiColor" ) )
    {
        G_DecolorString( m, m2 );
        G_Printf( m2 );
    }
    else
        G_Printf( m );
  }
}

void G_admin_buffer_begin()
{
  g_bfb[ 0 ] = '\0';
}

void G_admin_buffer_end( gentity_t *ent )
{
  ADMP( g_bfb );
}

void G_admin_buffer_print( gentity_t *ent, char *m )
{
  // 1022 - strlen("print 64 \"\"") - 1
  if( strlen( m ) + strlen( g_bfb ) >= 1009 )
  {
    ADMP( g_bfb );
    g_bfb[ 0 ] = '\0';
  }
  Q_strcat( g_bfb, sizeof( g_bfb ), m );
}


void G_admin_cleanup()
{
  int i = 0;

  for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
  {
    G_Free( g_admin_levels[ i ] );
    g_admin_levels[ i ] = NULL;
  }
  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
  {
    G_Free( g_admin_admins[ i ] );
    g_admin_admins[ i ] = NULL;
  }
  for( i = 0; i < MAX_ADMIN_BANS && g_admin_bans[ i ]; i++ )
  {
    G_Free( g_admin_bans[ i ] );
    g_admin_bans[ i ] = NULL;
  }
  for( i = 0; i < MAX_ADMIN_COMMANDS && g_admin_commands[ i ]; i++ )
  {
    G_Free( g_admin_commands[ i ] );
    g_admin_commands[ i ] = NULL;
  }
}

qboolean G_admin_L1(gentity_t *ent, int skiparg ){
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  int minargc;

  minargc = 2 + skiparg;

  if( G_SayArgc() < minargc )
  {
    ADMP( "^3!L1: ^7usage: !L1 [name]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!L1: ^7%s\n", err ) );
    return qfalse;
  }
  if( G_admin_level(&g_entities[ pids[ 0 ] ] )>0 )
  {
    ADMP( "^3!L1: ^7sorry, but that person is already higher than"
        " level 0.\n" );
    return qfalse;
  }
 
  trap_SendConsoleCommand( EXEC_APPEND,va( "!setlevel %d 1;", pids[ 0 ] ) );
  ClientUserinfoChanged( pids[ 0 ] );
  return qtrue;
}

//added commands

qboolean G_admin_forcespec( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  int minargc;
  gentity_t *vic;

  minargc = 2 + skiparg;

  if( G_SayArgc() < minargc )
  {
    ADMP( "^3!forcespec: ^7usage: !forcespec [name|slot#]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );

  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!forcespec: ^7%s\n", err ) );
    return qfalse;
  }

  vic = &g_entities[ pids[ 0 ] ];

  if ( vic->client->pers.specd == qtrue )
  {
    ADMP( "^3!forcespec: ^7player already forcespeced\n" );
    return qfalse;
  }
 
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( "^3!forcespec: ^7sorry, but your intended victim has a higher admin"
        " level than you\n" );
    return qfalse;
  }

  //push them to the spec team
  G_ChangeTeam( vic, PTE_NONE );

  //tell g_cmds that they cannot join teams
  vic->client->pers.specd = qtrue;

  CPx( pids[ 0 ], "cp \"^1You can no longer join teams\"" );
  AP( va( "print \"^3!forcespec: ^7%s^7 has disallowed joining of teams for ^7%s\n\"",
          ( ent ) ? ent->client->pers.netname : "console", vic->client->pers.netname ) );

  //tell g_cmds to save their creds
  vic->client->pers.saved = qtrue;

  return qtrue;
}

qboolean G_admin_unforcespec( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  int minargc;
  gentity_t *vic;

  minargc = 2 + skiparg;

  if( G_SayArgc() < minargc )
  {
    ADMP( "^3!unforcespec: ^7usage: !unforcespec [name|slot#]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );

  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!unforcespec: ^7%s\n", err ) );
    return qfalse;
  }

  vic = &g_entities[ pids[ 0 ] ];

  if ( vic->client->pers.specd == qfalse )
  {
    ADMP( "^3!unforcespec: ^7player is not forcespeced\n" );
    return qfalse;
  }

  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( "^3!forcespec: ^7sorry, but your intended victim has a higher admin"
        " level than you\n" );
    return qfalse;
  }

  //remove the limitation
  vic->client->pers.specd = qfalse;

  CPx( pids[ 0 ], "cp \"^1You can now join teams\"" );
  AP( va( "print \"^3!unforcespec: ^7%s^7 has allowed joining of teams for ^7%s\n\"",
          ( ent ) ? ent->client->pers.netname : "console", vic->client->pers.netname ) );

  return qtrue;
}

static qboolean G_admin_global_pause( gentity_t *ent, int skiparg )
{
  if( level.paused )
  {
    level.pauseTime = level.time;
    ADMP( "^3!pause: ^7Game is already paused, unpause timer reset\n" );
    return qfalse;
  }

  level.paused = qtrue;
  level.pauseTime = level.time;

  level.pause_speed = g_speed.value;
  level.pause_gravity = g_gravity.value;
  level.pause_knockback = g_knockback.value;
  level.pause_ff = g_friendlyFire.integer;
  level.pause_ffb = g_friendlyBuildableFire.integer;

  g_speed.value = 0;
  g_gravity.value = 0;
  g_knockback.value = 0;
  g_friendlyFire.integer = 0;
  g_friendlyBuildableFire.integer = 0;

  CP( "cp \"^1Game is PAUSED\"" );
  AP( va( "print \"^3!pause: ^7The game has been paused by %s\n\"",
    ( ent ) ? ent->client->pers.netname : "console" ) );

  return qtrue;
}

static qboolean G_admin_global_unpause( gentity_t *ent, int skiparg )
{
  if( !level.paused )
  {
    ADMP( "^3!unpause: ^7Game is not paused\n" );
    return qfalse;
  }

  level.paused = qfalse;

  g_speed.value = level.pause_speed;
  g_gravity.value = level.pause_gravity;
  g_knockback.value = level.pause_knockback;
  g_friendlyFire.integer = level.pause_ff;
  g_friendlyBuildableFire.integer = level.pause_ffb;

  CP( "cp \"^2Game is RESUMED\"" );
  AP( va( "print \"^3!unpause: ^7The game has been resumed by %s\n\"",
    ( ent ) ? ent->client->pers.netname : "console" ) );

  return qtrue;
}

qboolean G_admin_pause( gentity_t *ent, int skiparg )
{ //this is all very similar to denybuild, 
  //it performs an essentially identical function
  int i, j = 0, pids[ MAX_CLIENTS + 1 ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  char command[ MAX_ADMIN_CMD_LEN ], *cmd;
  qboolean targeted = qfalse;
  //targeted is set to ensure we don't get spam when pausing everybody
  gentity_t *vic;

  G_SayArgv( skiparg, command, sizeof( command ) );
  cmd = command;
  if( cmd && *cmd == '!' )
    cmd++;

  if( G_SayArgc() < 2 + skiparg )
  {
    // global pause
    if( !Q_stricmp( cmd, "pause" ) )
    {
      return G_admin_global_pause( ent, skiparg );
    }
    else
    {
      return G_admin_global_unpause( ent, skiparg );
    }
  }
  if( G_SayArgc() != 2 + skiparg )
  {
    ADMP( va( "^3!%s: ^7usage: !%s (^5name|slot^7)\n", cmd, cmd ) );
    return qfalse;
  }

  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( !Q_stricmp( name, "*" ) )
  {
    for( i = 0; i < MAX_CLIENTS; i++ )
    {
      vic = &g_entities[ i ];
      if( vic && vic->client &&
          vic->client->pers.connected == CON_CONNECTED )
      {
	pids[ j ] = i;
        j++;
      }
    }
    pids[ j ] = -1;
  }
  else
  {
    if( G_ClientNumbersFromString( name, pids ) != 1 )
    {
      G_MatchOnePlayer( pids, err, sizeof( err ) );
      ADMP( va( "^3!%s: ^7%s\n", cmd, err ) );
      return qfalse;
    }
    targeted = qtrue;
  }

  for( i = 0; pids[ i ] >= 0; i++ )
  {
    vic = &g_entities[ pids[ i ] ];
    if ( !vic || !vic->client ) continue;
    if( !admin_higher( ent, vic ) )
    {
      if( targeted )
	ADMP( va( "^3!%s: ^7sorry, but your intended victim has a higher admin"
		    " level than you\n", cmd ) );
      continue;
    }
    if( vic->client->pers.paused )
    {
      if( !Q_stricmp( cmd, "pause" ) )
      {
        if( targeted )
	  ADMP( "^3!pause: ^7player is already paused\n" );
	continue;
      }
      vic->client->pers.paused = qfalse;
      CPx( pids[ i ], "cp \"^2You've been unpaused\"" );
      if( targeted )
	AP( va( "print \"^3!unpause: ^7%s^7 unpaused by %s\n\"",
	    vic->client->pers.netname,
	    ( ent ) ? ent->client->pers.netname : "console" ) );
    }
    else
    {
      if( !Q_stricmp( cmd, "unpause" ) )
      {
	if( targeted )
	  ADMP( "^3!unpause: ^7player is already unpaused\n" );
	continue;
      }
      vic->client->pers.paused = qtrue;
      CPx( pids[ i ], va( "cp \"^1You've been paused by ^7%s\"", 
	  ( ent ) ? ent->client->pers.netname : "console" ) );
      if( targeted )
	AP( va( "print \"^3!pause: ^7%s^7 paused by %s\n\"", 
	    vic->client->pers.netname,
            ( ent ) ? ent->client->pers.netname : "console" ) );
    }
    ClientUserinfoChanged( pids[ i ] );
  }
  return qtrue;
}

qboolean G_admin_practice( gentity_t *ent, int skiparg )
{
  char clantag[ MAX_NAME_LENGTH ];
  char mapsarg[ 8 ];
  int maps;

  if( G_SayArgc() < 2 + skiparg )
  {
    if( g_practiceCount.integer )
    {
      AP( va( "print \"^3practice:^7 practice mode is in effect for the next %d maps\n\"",
        g_practiceCount.integer ) );
      ADMP( va( "^3!practice: ^7practice mode set to %s^7 for next %d maps\n",
        g_practiceText.string, g_practiceCount.integer ) );
    }
    else
    {
      ADMP( "^3!practice: ^7practice mode is off\n" );
    }
    return qfalse;
  }

  G_SayArgv( 1 + skiparg, clantag, sizeof( clantag ) );

  if( G_SayArgc() < 3 + skiparg )
  {
    if( !Q_stricmp( clantag, "off" ) )
    {
      if( g_practiceCount.integer )
      {
        trap_Cvar_Set( "g_practiceCount", "0" );
        AP( va ("print \"^3!practice: ^7 practice mode turned off by %s^7\n\"",
          ( ent ) ? ent->client->pers.netname : "console" ) );
        ADMP( "^3!practice: ^7practice mode set to off\n" );
        return qtrue;
      }
      else
      {
        ADMP( "^3!practice: ^7practice mode already off\n" );
      }
    }
    else
    {
      ADMP( "^3!practice: ^7usage: practice [clan tag] [map count]\n" );
    }
    return qfalse;
  }

  G_SayArgv( 2 + skiparg, mapsarg, sizeof( mapsarg ) );
  maps = atoi( mapsarg );

  if( !clantag[ 0 ] )
  {
    ADMP( "^3!practice: ^7no clan tag specified\n" );
    return qfalse;
  }
  if( maps < 1 || maps > 8  )
  {
    ADMP( "^3!practice: ^7map count must be between 1 and 8\n" );
    return qfalse;
  }

  trap_Cvar_Set( "g_practiceText", clantag );
  trap_Cvar_Set( "g_practiceCount", va( "%d", maps ) );

  AP( va( "print \"^3practice:^7 %s^7 has activated practice mode for the next %d maps\n\"",
      ( ent ) ? ent->client->pers.netname : "console",
      maps ) );

  return qtrue;
}

qboolean G_admin_buildlog( gentity_t *ent, int skiparg )
{
#define LOG_DISPLAY_LENGTH 10
  buildHistory_t *ptr;
  gentity_t *builder = NULL;
  int skip = 0, start = 0, lastID = -1, firstID = -1, i, len, matchlen = 0;
  pTeam_t team = PTE_NONE;
  char message[ MAX_STRING_CHARS ], *teamchar; 
  char *name, *action, *buildablename, markstring[ MAX_STRING_CHARS ]; 
  if( !g_buildLogMaxLength.integer )
  {
    ADMP( "^3!buildlog: ^7build logging is disabled" );
    return qfalse;
  }
  if( G_SayArgc( ) >= 2 + skiparg )
  {
    for( i = 1; i + skiparg < G_SayArgc( ); i++ )
    {
      char argbuf[ 64 ], err[ MAX_STRING_CHARS ];
      int x = 0, pids[ MAX_CLIENTS ];
      G_SayArgv( i + skiparg, argbuf, sizeof argbuf );
      switch( argbuf[ 0 ])
      {
        case 'x':
          x = 1;
        default:
          skip = atoi( argbuf + x );
          start = 0;
          break;
        case '#':
          start = atoi( argbuf + 1 );
          skip = 0;
          break;
        case '-':
          {
            if( G_ClientNumbersFromString( argbuf + 1, pids ) != 1 )
            {
              G_MatchOnePlayer( pids, err, sizeof( err ) );
              ADMP( va( "^3!buildlog: ^7%s\n", err ));
              return qfalse;
            }
            builder = g_entities + *pids;
          }
          break;
        case 'A':
        case 'a':
          team = PTE_ALIENS;
          break;
        case 'H':
        case 'h':
          team = PTE_HUMANS;
          break;
      }
    }
  }
  // !buildlog can be abused, so let everyone know when it is used
  AP( va( "print \"^3!buildlog: ^7%s^7 requested a log of recent building"
      " activity\n\"", ( ent ) ? ent->client->pers.netname : "console" ) );
  len = G_CountBuildLog( ); // also clips the log if too long
  if( !len )
  {
    ADMP( "^3!buildlog: ^7no build log found\n" );
    return qfalse;
  }
  if( start )
  {
    // set skip based on start
    for( ptr = level.buildHistory; ptr && ptr->ID != start; 
        ptr = ptr->next, skip++ );
    if( !ptr )
    {
      ADMP( "^3!buildlog: ^7log ID not found\n" );
      skip = 0;
    }
  }
  // ensure skip is a useful value
  if( skip > len - LOG_DISPLAY_LENGTH )
    skip = len - LOG_DISPLAY_LENGTH;
  *message = '\0';
  // skip to start entry
  for( ptr = level.buildHistory, i = len; ptr && i > len - skip; 
      ptr = ptr->next )
  {
    // these checks could perhaps be done more efficiently but they are cheap
    // in processor time so I'm not worrying
    if( team != PTE_NONE && team != BG_FindTeamForBuildable( ptr->buildable ) )
      continue;
    if( builder && builder != ptr->ent )
      continue;
    matchlen++;
    i--;
  }
  for( ; i + LOG_DISPLAY_LENGTH > len - skip && i > 0; i--, ptr = ptr->next )
  {
    if( !ptr ) 
      break; // run out of log
    *markstring = '\0'; // reinit markstring
    // check team
    if( ( team != PTE_NONE && team != BG_FindTeamForBuildable( ptr->buildable ) )
        || ( builder && builder != ptr->ent ) )
    {
      skip++; // loop an extra time because we skipped one
      continue; 
    }
    if( lastID < 0 )
      lastID = ptr->ID;
    firstID = ptr->ID;
    matchlen++;
    // set name to the ent's current name or last recorded name
    if( ptr->ent )
    {
      if( ptr->ent->client )
        name = ptr->ent->client->pers.netname;
      else
        name = "<world>"; // any non-client action
    }
    else
      name = ptr->name;
    switch( ptr->fate )
    {
      case BF_BUILT:
        action = "^2built^7 a";
        break;
      case BF_DECONNED:
        action = "^3DECONSTRUCTED^7 a";
        break;
      case BF_DESTROYED:
        action = "destroyed a";
        break;
      case BF_TEAMKILLED:
        action = "^1TEAMKILLED^7 a";
        break;
      default:
        action = "\0"; // erm
        break;
    }
    // handle buildables removed by markdecon
    if( ptr->marked )
    {
      buildHistory_t *mark;
      int j, markdecon[ BA_NUM_BUILDABLES ], and = 2;
      char bnames[ 32 ], *article;
      mark = ptr;
      // count the number of buildables
      memset( markdecon, 0, sizeof( markdecon ) );
      while( ( mark = mark->marked ) )
        markdecon[ mark->buildable ]++;
      // reverse order makes grammar easier
      for( j = BA_NUM_BUILDABLES; j >= 0; j-- )
      {
        buildablename = BG_FindHumanNameForBuildable( j );
        // plural is easy
        if( markdecon[ j ] > 1 )
          Com_sprintf( bnames, 32, "%d %ss", markdecon[ j ], buildablename );
        // use an appropriate article
        else if( markdecon[ j ] == 1 )
        {
          if( BG_FindUniqueTestForBuildable( j ) )
            article = "the"; // if only one
          else if( strchr( "aeiouAEIOU", *buildablename ) )
            article = "an"; // if first char is vowel
          else
            article = "a";
          Com_sprintf( bnames, 32, "%s %s", article, buildablename );
        }
        else
          continue; // none of this buildable
        // C grammar: x, y, and z
        // the integer and is 2 initially, the test means it is used on the
        // second sprintf only, the reverse order makes this second to last
        // the comma is only printed if there is already some markstring i.e.
        // not the first time ( which would put it on the end of the string )
        Com_sprintf( markstring, sizeof( markstring ), "%s%s %s%s", bnames, 
            ( *markstring ) ? "," : "", ( and-- == 1 ) ? "and " : "", markstring );
      }
    }
    buildablename = BG_FindHumanNameForBuildable( ptr->buildable );
    switch( BG_FindTeamForBuildable( ptr->buildable ) )
    {
      case PTE_ALIENS: 
        teamchar = "^1A"; 
        break;
      case PTE_HUMANS: 
        teamchar = "^4H"; 
        break;
      default: 
        teamchar = " "; // space so it lines up neatly
        break;
    }
    // prepend the information to the string as we go back in buildhistory
    // so the earliest events are at the top
    Com_sprintf( message, MAX_STRING_CHARS, "%3d %s^7 %s^7 %s%s %s%s%s\n%s", 
        ptr->ID, teamchar, name, action, 
        ( strchr( "aeiouAEIOU", buildablename[ 0 ] ) ) ? "n" : "", 
        buildablename, ( markstring[ 0 ] ) ? ", removing " : "", 
        markstring, message );
  }
  for( ; ptr; ptr = ptr->next )
  {
    if( builder && builder != ptr->ent )
      continue;
    if( team != PTE_NONE && team != BG_FindTeamForBuildable( ptr->buildable ) )
      continue;
    matchlen++;
  }
  if( matchlen )
    ADMP( va( "%s^3!buildlog: showing log entries %d - %d of %d\n", message,
        firstID, lastID, matchlen ) );
  else
    ADMP( "^3!buildlog: ^7no log entries match those criteria\n" );
  return qtrue;
}

qboolean G_admin_revert( gentity_t *ent, int skiparg )
{
  int i = 0, j = 0, repeat = 1, ID = 0, len, matches = 0;
  pTeam_t team = PTE_NONE;
  qboolean force = qfalse, reached = qfalse;
  gentity_t *builder = NULL, *targ;
  buildHistory_t *ptr, *tmp, *mark, *prev;
  vec3_t dist;
  char argbuf[ 64 ], *name, *bname, *action, *article;
  len = G_CountBuildLog( );
  if( !len )
  {
    ADMP( "^3!revert: ^7no build log found\n" );
    return qfalse;
  }
  if( G_SayArgc( ) < 2 + skiparg )
  {
    ADMP( "^3!revert: ^7usage: !revert (^5xnum^7) (^5#ID^7) (^5-name|num^7) (^5a|h^7)\n" );
    return qfalse;
  }
  for( i = 1; i + skiparg < G_SayArgc( ); i++ )
  {
    char arg[ 64 ], err[ MAX_STRING_CHARS ];
    int pids[ MAX_CLIENTS ];
    G_SayArgv( i + skiparg, arg, sizeof arg );
    switch( arg[ 0 ])
    {
      case 'x':
        repeat = atoi( arg + 1 );
        break;
      case '#':
        ID = atoi( arg + 1 );
        break;
      case '-':
        {
          if( G_ClientNumbersFromString( arg + 1, pids ) != 1 )
          {
            G_MatchOnePlayer( pids, err, sizeof( err ) );
            ADMP( va( "^3!revert: ^7%s\n", err ));
            return qfalse;
          }
          builder = g_entities + *pids;
        }
        break;
      case 'A':
      case 'a':
        team = PTE_ALIENS;
        break;
      case 'H':
      case 'h':
        team = PTE_HUMANS;
        break;
      case '!':
        force = qtrue;
        break;
      default:
        ADMP( "^3!revert: ^7usage: !revert (^5xnum^7) (^5#ID^7) (^5-name|num^7) (^5a|h^7)\n" );
        return qfalse;
    }
  }
  if( repeat > 25 )
  {
    ADMP( "^3!revert: ^7to avoid flooding, can only revert 25 builds at a time\n" );
    repeat = 25;
  }
  for( i = 0, ptr = prev = level.buildHistory; repeat > 0; repeat--, j = 0 )
  {
    if( !ptr ) 
      break; // run out of bhist
    if( !reached && ID )
    {
      if( ptr->ID == ID )
        reached = qtrue;
      else
      {
        prev = ptr;
        ptr = ptr->next;
        repeat++;
        continue;
      }
    }
    if( ( team != PTE_NONE && 
        team != BG_FindTeamForBuildable( ptr->buildable ) ) || 
        ( builder && builder != ptr->ent ))
    {
      // team doesn't match, so skip this ptr and reset prev
      prev = ptr;
      ptr = ptr->next;
      // we don't want to count this one so counteract the decrement by the for
      repeat++;
      continue;
    }
    // get the ent's current or last recorded name
    if( ptr->ent )
    {
      if( ptr->ent->client )
        name = ptr->ent->client->pers.netname;
      else
        name = "<world>"; // non-client actions
    }
    else
      name = ptr->name;
    bname = BG_FindHumanNameForBuildable( ptr->buildable ); 
    action = "";
    switch( ptr->fate )
    {
      case BF_BUILT:
        action = "build";
        for( j = MAX_CLIENTS, targ = g_entities + j;
            j < level.num_entities; j++, targ++ )
        {
          // easy checks first to save time
          if( targ->s.eType != ET_BUILDABLE )
            continue;
          if( targ->s.modelindex != ptr->buildable )
            continue; 
          VectorSubtract( targ->s.pos.trBase, ptr->origin, dist );
#define FIND_BUILDABLE_TOLERANCE 5
          if( VectorLength( dist ) > FIND_BUILDABLE_TOLERANCE )
            continue; // number is somewhat arbitrary, watch for false pos/neg
          // if we didn't continue then it's this one, unlink it but we can't
          // free it yet, because the markdecon buildables might not place
          trap_UnlinkEntity( targ );
          break;
        }
        // if there are marked buildables to replace, and we aren't overriding 
        // space check, check they can fit before acting
        if( ptr->marked && !force )
        {
          for( mark = ptr->marked; mark; mark = mark->marked )
            if( !G_RevertCanFit( mark ) )
            {
              trap_LinkEntity( targ ); // put it back, we failed
              // scariest sprintf ever:
              Com_sprintf( argbuf, sizeof argbuf, "%s%s%s%s%s%s%s!",
                  ( repeat > 1 ) ? "x" : "", ( repeat > 1 ) ? va( "%d ", repeat ) : "",
                  ( ID ) ? "#" : "", ( ID ) ? va( "%d ", ptr->ID ) : "",
                  ( builder ) ? "-" : "", ( builder ) ? va( "%i ", (int)(builder - g_entities) ) : "",
                  ( team == PTE_ALIENS ) ? "a " : ( team == PTE_HUMANS ) ? "h " : "" );
              ADMP( va( "^3!revert: ^7revert aborted: reverting this %s would conflict with "
                  "another buildable, use ^3!revert %s ^7to override\n", action, argbuf ) );
              return qfalse;
            }
        }

        // Prevent teleport glitch when reverting an occupied hovel
        if( targ->s.modelindex == BA_A_HOVEL && targ->active )
        {
            gentity_t *builder = targ->builder;
            vec3_t    newOrigin;
            vec3_t    newAngles;

            VectorCopy( targ->s.angles, newAngles );
            newAngles[ ROLL ] = 0;

            VectorCopy( targ->s.origin, newOrigin );
            VectorMA( newOrigin, 1.0f, targ->s.origin2, newOrigin );

            //prevent lerping
            builder->client->ps.eFlags ^= EF_TELEPORT_BIT;
            builder->client->ps.eFlags &= ~EF_NODRAW;
            G_UnlaggedClear( builder );

            G_SetOrigin( builder, newOrigin );
            VectorCopy( newOrigin, builder->client->ps.origin );
            G_SetClientViewAngle( builder, newAngles );

            //client leaves hovel
            builder->client->ps.stats[ STAT_STATE ] &= ~SS_HOVELING;
        }

        // if we haven't returned yet then we're good to go, free it
        G_FreeEntity( targ );
        // put the marked buildables back and mark them again
        if( ptr->marked ) // there may be a more efficient way of doing this
        {
          for( mark = ptr->marked; mark; mark = mark->marked )
            G_SpawnRevertedBuildable( mark, qtrue ); 
        }
        break;
      case BF_DECONNED:
        action = "deconstruction";
      case BF_DESTROYED:
      case BF_TEAMKILLED:
        if( !action[ 0 ] ) 
          action = "destruction";
        // if we're not overriding and the replacement can't fit, as before
        if( !force && !G_RevertCanFit( ptr ) )
        {
          Com_sprintf( argbuf, sizeof argbuf, "%s%s%s%s%s%s%s!",
              ( repeat > 1 ) ? "x" : "", ( repeat > 1 ) ? va( "%d ", repeat ) : "",
              ( ID ) ? "#" : "", ( ID ) ? va( "%d ", ptr->ID ) : "",
              ( builder ) ? "-" : "", ( builder ) ? va( "%i ", (int)(builder - g_entities) ) : "",
              ( team == PTE_ALIENS ) ? "a " : ( team == PTE_HUMANS ) ? "h " : "" );
          ADMP( va( "^3!revert: ^7revert aborted: reverting this %s would "
              "conflict with another buildable, use ^3!revert %s ^7to override\n",
              action, argbuf ) );
          return qfalse;
        }
        // else replace it but don't mark it ( it might have been marked before
        // but it isn't that important )
        G_SpawnRevertedBuildable( ptr, qfalse );
        break;
      default:
        // if this happens something has gone wrong
        ADMP( "^3!revert: ^7incomplete or corrupted build log entry\n" );
        /* quarantine and dispose of the log, it's dangerous
        trap_Cvar_Set( "g_buildLogMaxLength", "0" );
        G_CountBuildLog( );
        */
        return qfalse;
    }
    if( j == level.num_entities )
    {
      ADMP( va( "^3!revert: ^7could not find logged buildable #%d\n", ptr->ID ));
      prev = ptr;
      ptr = ptr->next;
      continue;
    }
    // this is similar to the buildlog stuff
    if( BG_FindUniqueTestForBuildable( ptr->buildable ) )
      article = "the";
    else if( strchr( "aeiouAEIOU", *bname ) )
      article = "an";
    else
      article = "a";
    AP( va( "print \"%s^7 reverted %s^7'%s %s of %s %s\n\"", 
        ( ent ) ? ent->client->pers.netname : "console",
        name, strchr( "Ss", name[ strlen( name ) - 1 ] ) ? "" : "s",
        action, article, bname ) ); 
	matches++;
    // remove the reverted entry
    // ptr moves on, prev just readjusts ->next unless it is about to be 
    // freed, in which case it is forced to move on too
    tmp = ptr;
    if( ptr == level.buildHistory )
      prev = level.buildHistory = ptr = ptr->next;
    else
      prev->next = ptr = ptr->next;
    G_Free( tmp );
  }
  if( ID && !reached )
    ADMP( "^3!revert: ^7no buildlog entry with that ID\n" );
  else if( matches > 0 )
  {
	ADMP( va( "^3!revert: ^7reverted %d buildlog events\n", matches ) );
    return qtrue;
  }
  else
	ADMP( "^3!revert: ^7no buildlog entries match those criteria\n" );
  return qfalse;
}

qboolean G_admin_drop( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  int minargc;
  char msg[ MAX_STRING_CHARS ];
  char *s;
  minargc = 2 + skiparg;

  if( G_SayArgc() < minargc )
  {
    ADMP( "^3!drop: ^7usage: !drop [name|slot#] [message]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );

  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!drop: ^7%s\n", err ) );
    return qfalse;
  }
  
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( "^3!drop: ^7sorry, but your intended victim has a higher admin"
        " level than you\n" );
    return qfalse;
  }
  
  minargc = 3 + skiparg;
  
  s = G_SayConcatArgs( 2 + skiparg );
  
  Q_strncpyz( msg, s, sizeof( msg ) );
  
  //what they get
  if( G_SayArgc() < minargc )
    trap_SendServerCommand( pids[ 0 ], va( "disconnect" ) );
  else
    trap_SendServerCommand( pids[ 0 ], va( "disconnect \"You have been dropped.\n\n%s^7\n\"", msg ) );

  trap_DropClient( pids[ 0 ], va( "disconnected" ) );

  return qtrue;
}

qboolean G_admin_cp( gentity_t *ent, int skiparg )
{

  char msg[ MAX_STRING_CHARS ];
  int minargc;
  char *s;

  minargc = 2 + skiparg;

  if( G_SayArgc() < minargc )
  {
    ADMP( "^3!cp: ^7usage: !cp [message]\n" );
    return qfalse;
  }

  s = G_SayConcatArgs( 1 + skiparg );
  Q_strncpyz( msg, s, sizeof( msg ) );
  G_ParseEscapedString( msg );
  trap_SendServerCommand( -1, va( "cp \"%s\"", msg ) );
  trap_SendServerCommand( -1, va( "print \"CP: %s\n\"", msg ) );

  return qtrue;
}

qboolean G_admin_L0(gentity_t *ent, int skiparg ){
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ] = {""};
  char testname[ MAX_NAME_LENGTH ] = {""};
  char err[ MAX_STRING_CHARS ];
  qboolean numeric = qtrue;
  int i;
  int id = -1;
  gentity_t *vic;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!L0: ^7usage: !L0 [name]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, testname, sizeof( testname ) );
  G_SanitiseString( testname, name, sizeof( name ) );
  for( i = 0; i < sizeof( name ) && name[ i ] ; i++ )
  {
    if( name[ i ] < '0' || name[ i ] > '9' )
    {
      numeric = qfalse;
      break;
    }
  }

  if( numeric )
  {
    id = atoi( name );
  }
  else
  {
    if( G_ClientNumbersFromString( name, pids ) != 1 )
    {
      G_MatchOnePlayer( pids, err, sizeof( err ) );
      ADMP( va( "^3!L0: ^7%s\n", err ) );
      return qfalse;
    }
    id = pids[ 0 ];
  }

  if (id >= 0 && id < level.maxclients)
  {
    vic = &g_entities[ id ];
    if( !vic || !(vic->client) || vic->client->pers.connected != CON_CONNECTED )
    {
      ADMP( "^3!L0:^7 no one connected by that slot number\n" );
      return qfalse;
    }

    if( G_admin_level( vic ) != 1 )
    {
      ADMP( "^3!L0:^7 intended victim is not level 1\n" );
      return qfalse;
    }
  }
  else if (id >= MAX_CLIENTS && id < MAX_CLIENTS + MAX_ADMIN_ADMINS
    && g_admin_admins[ id - MAX_CLIENTS ] )
  {
    if( g_admin_admins[ id - MAX_CLIENTS ]->level != 1 )
    {
      ADMP( "^3!L0:^7 intended victim is not level 1\n" );
      return qfalse;
    }
  }
  else
  {
    ADMP( "^3!L0:^7 no match.  use !listplayers or !listadmins to "
      "find an appropriate number to use instead of name.\n" );
    return qfalse;
  }

  trap_SendConsoleCommand( EXEC_APPEND, va( "!setlevel %d 0;", id ) );

  return qtrue;
}

qboolean G_admin_seen(gentity_t *ent, int skiparg )
{
  char name[ MAX_NAME_LENGTH ];
  char search[ MAX_NAME_LENGTH ];
  char sduration[ 32 ];
  qboolean numeric = qtrue;
  int i, j;
  int id = -1;
  int count = 0;
  int t;
  qtime_t qt;
  gentity_t *vic;
  qboolean ison;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!seen: ^7usage: !seen [name|admin#]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  G_SanitiseString( name, search, sizeof( search ) );
  for( i = 0; i < sizeof( search ) && search[ i ] ; i++ )
  {
    if( search[ i ] < '0' || search[ i ] > '9' )
    {
      numeric = qfalse;
      break;
    }
  }

  if( numeric )
  {
    id = atoi( name );
    search[ 0 ] = '\0';
  }

  ADMBP_begin();
  t = trap_RealTime( &qt );

  for( i = 0; i < level.maxclients && count < 10; i ++ )
  {
    vic = &g_entities[ i ];

    if( !vic->client || vic->client->pers.connected != CON_CONNECTED )
      continue;

    G_SanitiseString( vic->client->pers.netname, name, sizeof( name ) );

    if( i == id || (search[ 0 ] && strstr( name, search ) ) )
    {
      ADMBP( va( "%4d %s^7 is currently playing\n", i, vic->client->pers.netname ) );
      count++;
    }
  }
  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ] && count < 10; i++ )
  {
    G_SanitiseString( g_admin_admins[ i ]->name, name, sizeof( name ) );
    if( i + MAX_CLIENTS == id || (search[ 0 ] && strstr( name, search ) ) )
    {
      ison = qfalse;
      for( j = 0; j < level.maxclients; j++ )
      {
        vic = &g_entities[ j ];
        if( !vic->client || vic->client->pers.connected != CON_CONNECTED )
          continue;
        G_SanitiseString( vic->client->pers.netname, name, sizeof( name ) );
        if( !Q_stricmp( vic->client->pers.guid, g_admin_admins[ i ]->guid )
          && strstr( name, search ) )
        {
            if( vic->client->sess.invisible == qfalse )
            {
                ison = qtrue;
                break;
            }
        }
      }

      if( ison )
      {
        if( id == -1 )
          continue;
        ADMBP( va( "%4d %s^7 is currently playing\n",
          i + MAX_CLIENTS, g_admin_admins[ i ]->name ) );
      }
      else
      {
        G_admin_duration( t - g_admin_admins[ i ]->seen,
          sduration, sizeof( sduration ) );
        ADMBP( va( "%4d %s^7 last seen %s%s\n",
          i + MAX_CLIENTS, g_admin_admins[ i ]->name ,
          ( g_admin_admins[ i ]->seen ) ? sduration : "",
          ( g_admin_admins[ i ]->seen ) ? " ago" : "time is unknown" ) );
      }
      count++;
    }
  }

  if( search[ 0 ] )
    ADMBP( va( "^3!seen:^7 found %d player%s matching '%s'\n",
      count, (count == 1) ? "" : "s", search ) );
  else if ( !count )
    ADMBP( "^3!seen:^7 no one connectd by that slot number\n" );

  ADMBP_end();
  return qtrue;
}

void G_admin_seen_update( char *guid )
{
  int i;

  for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ] ; i++ )
  {
    if( !Q_stricmp( g_admin_admins[ i ]->guid, guid ) )
    {
      qtime_t qt;

      g_admin_admins[ i ]->seen = trap_RealTime( &qt );
      return;
    }
  }
}

void G_admin_adminlog_cleanup( void )
{
  int i;

  for( i = 0; i < MAX_ADMIN_ADMINLOGS && g_admin_adminlog[ i ]; i++ )
  {
    G_Free( g_admin_adminlog[ i ] );
    g_admin_adminlog[ i ] = NULL;
  }

  admin_adminlog_index = 0;
}

void G_admin_adminlog_log( gentity_t *ent, char *command, char *args, int skiparg, qboolean success )
{
  g_admin_adminlog_t *adminlog;
  int previous;
  int count = 1;
  int i;

  if( !command )
    return;

  if( !Q_stricmp( command, "adminlog" ) ||
      !Q_stricmp( command, "admintest" ) ||
      !Q_stricmp( command, "help" ) ||
      !Q_stricmp( command, "info" ) ||
      !Q_stricmp( command, "listadmins" ) ||
      !Q_stricmp( command, "listplayers" ) ||
      !Q_stricmp( command, "namelog" ) ||
      !Q_stricmp( command, "showbans" ) ||
      !Q_stricmp( command, "seen" ) ||
      !Q_stricmp( command, "time" ) )
    return;

  previous = admin_adminlog_index - 1;
  if( previous < 0 )
    previous = MAX_ADMIN_ADMINLOGS - 1;

  if( g_admin_adminlog[ previous ] )
    count = g_admin_adminlog[ previous ]->id + 1;

  if( g_admin_adminlog[ admin_adminlog_index ] )
    adminlog = g_admin_adminlog[ admin_adminlog_index ];
  else
    adminlog = G_Alloc( sizeof( g_admin_adminlog_t ) );

  memset( adminlog, 0, sizeof( adminlog ) );
  adminlog->id = count;
  adminlog->time = level.time - level.startTime;
  adminlog->success = success;
  Q_strncpyz( adminlog->command, command, sizeof( adminlog->command ) );

  if ( args )
    Q_strncpyz( adminlog->args, args, sizeof( adminlog->args ) );
  else
    Q_strncpyz( adminlog->args, G_SayConcatArgs( 1 + skiparg ), sizeof( adminlog->args ) );

  if( ent )
  {
    qboolean found = qfalse;
    // real admin name
    for( i = 0; i < MAX_ADMIN_ADMINS && g_admin_admins[ i ]; i++ )
    {
      if( !Q_stricmp( g_admin_admins[ i ]->guid, ent->client->pers.guid ) )
      {
        Q_strncpyz( adminlog->name, g_admin_admins[ i ]->name, sizeof( adminlog->name ) );
        found = qtrue;
        break;
      }
    }
    if( !found )
      Q_strncpyz( adminlog->name, ent->client->pers.netname, sizeof( adminlog->name ) );
    adminlog->level = ent->client->pers.adminLevel;
  }
  else
  {
    Q_strncpyz( adminlog->name, "console", sizeof( adminlog->name ) );
    adminlog->level = 10000;
  }

  g_admin_adminlog[ admin_adminlog_index ] = adminlog;
  admin_adminlog_index++;
  if( admin_adminlog_index >= MAX_ADMIN_ADMINLOGS )
    admin_adminlog_index = 0;
}

qboolean G_admin_adminlog( gentity_t *ent, int skiparg )
{
  g_admin_adminlog_t *results[ 10 ];
  int result_index = 0;
  char *search_cmd = NULL;
  char *search_name = NULL;
  int index;
  int skip = 0;
  int skipped = 0;
  int checked = 0;
  char n1[ MAX_NAME_LENGTH ];
  char fmt_name[ 16 ];
  char argbuf[ 32 ];
  int name_length = 12;
  int max_id = 0;
  int i;
  qboolean match;

  memset( results, 0, sizeof( results ) );

  index = admin_adminlog_index;
  for( i = 0; i < 10; i++ )
  {
    int prev;

    prev = index - 1;
    if( prev < 0 )
      prev = MAX_ADMIN_ADMINLOGS - 1;
    if( !g_admin_adminlog[ prev ] )
      break;
    if( g_admin_adminlog[ prev ]->id > max_id )
      max_id = g_admin_adminlog[ prev ]->id;
    index = prev;
  }

  if( G_SayArgc() > 1 + skiparg )
  {
    G_SayArgv( 1 + skiparg, argbuf, sizeof( argbuf ) );
    if( ( *argbuf >= '0' && *argbuf <= '9' ) || *argbuf == '-' )
    {
      int id;

      id = atoi( argbuf );
      if( id < 0 )
        id += ( max_id - 9 );
      else if( id <= max_id - MAX_ADMIN_ADMINLOGS )
        id = max_id - MAX_ADMIN_ADMINLOGS + 1;

      if( id + 9 >= max_id )
        id = max_id - 9;
      if( id < 1 )
        id = 1;
      for( i = 0; i < MAX_ADMIN_ADMINLOGS; i++ )
      {
        if( g_admin_adminlog[ i ]->id == id )
        {
          index = i;
          break;
        }
      }
    }
    else if ( *argbuf == '!' )
    {
      search_cmd = argbuf + 1;
    }
    else
    {
      search_name = argbuf;
    }

    if( G_SayArgc() > 2 + skiparg && ( search_cmd || search_name ) )
    {
      char skipbuf[ 4 ];
      G_SayArgv( 2 + skiparg, skipbuf, sizeof( skipbuf ) );
      skip = atoi( skipbuf );
    }
  }

  if( search_cmd || search_name )
  {
    g_admin_adminlog_t *result_swap[ 10 ];

    memset( result_swap, 0, sizeof( result_swap ) );

    index = admin_adminlog_index - 1;
    if( index < 0 )
      index = MAX_ADMIN_ADMINLOGS - 1;

    while( g_admin_adminlog[ index ] &&
      checked < MAX_ADMIN_ADMINLOGS &&
      result_index < 10 )
    {
      match = qfalse;
      if( search_cmd )
      {
        if( !Q_stricmp( search_cmd, g_admin_adminlog[ index ]->command ) )
          match = qtrue;
      }
      if( search_name )
      {
        G_SanitiseString( g_admin_adminlog[ index ]->name, n1, sizeof( n1 ) );
        if( strstr( n1, search_name ) )
          match = qtrue;
      }

      if( match && skip > 0 )
      {
        match = qfalse;
        skip--;
        skipped++;
      }
      if( match )
      {
        result_swap[ result_index ] = g_admin_adminlog[ index ];
        result_index++;
      }

      checked++;
      index--;
      if( index < 0 )
        index = MAX_ADMIN_ADMINLOGS - 1;
    }
    // search runs backwards, turn it around
    for( i = 0; i < result_index; i++ )
      results[ i ] = result_swap[ result_index - i - 1 ];
  }
  else
  {
    while( g_admin_adminlog[ index ] && result_index < 10 )
    {
      results[ result_index ] = g_admin_adminlog[ index ];
      result_index++;
      index++;
      if( index >= MAX_ADMIN_ADMINLOGS )
        index = 0;
    }
  }

  for( i = 0; results[ i ] && i < 10; i++ )
  {
    int l;

    G_DecolorString( results[ i ]->name, n1 );
    l = strlen( n1 );
    if( l > name_length )
      name_length = l;
  }
  ADMBP_begin( );
  for( i = 0; results[ i ] && i < 10; i++ )
  {
    char levelbuf[ 3 ];
    int t;

    t = results[ i ]->time / 1000;
    G_DecolorString( results[ i ]->name, n1 );
    Com_sprintf( fmt_name, sizeof( fmt_name ), "%%%ds", 
      (int)( name_length + strlen( results[ i ]->name ) - strlen( n1 ) ) );
    Com_sprintf( n1, sizeof( n1 ), fmt_name, results[ i ]->name );
    Com_sprintf( levelbuf, sizeof( levelbuf ), "%2d", results[ i ]->level );
    ADMBP( va( "%s%3d %3d:%02d %2s ^7%s^7 %s!%s ^7%s^7\n",
      ( results[ i ]->success ) ? "^7" : "^1",
      results[ i ]->id,
      t / 60, t % 60,
      ( results[ i ]->level ) < 10000 ? levelbuf : " -",
      n1,
      ( results[ i ]->success ) ? "^3" : "^1",
      results[ i ]->command,
      results[ i ]->args ) );
  }
  if( search_cmd || search_name )
  {
    ADMBP( va( "^3!adminlog:^7 Showing %d matches for '%s^7'.",
      result_index,
      argbuf ) );
    if( checked < MAX_ADMIN_ADMINLOGS && g_admin_adminlog[ checked ] )
      ADMBP( va( " run '!adminlog %s^7 %d' to see more",
       argbuf,
       skipped + result_index ) );
    ADMBP( "\n" );
  }
  else if ( results[ 0 ] )
  {
    ADMBP( va( "^3!adminlog:^7 Showing %d - %d of %d.\n",
      results[ 0 ]->id,
      results[ result_index - 1 ]->id,
      max_id ) );
  }
  else
  {
    ADMBP( "^3!adminlog:^7 log is empty.\n" );
  }
  ADMBP_end( );

  return qtrue;
}

void G_admin_tklog_cleanup( void )
{
  int i;

  for( i = 0; i < MAX_ADMIN_TKLOGS && g_admin_tklog[ i ]; i++ )
  {
    G_Free( g_admin_tklog[ i ] );
    g_admin_tklog[ i ] = NULL;
  }

  admin_tklog_index = 0;
}

void G_admin_tklog_log( gentity_t *attacker, gentity_t *victim, int meansOfDeath )
{
  g_admin_tklog_t *tklog;
  int previous;
  int count = 1;

  if( !attacker )
    return;

  previous = admin_tklog_index - 1;
  if( previous < 0 )
    previous = MAX_ADMIN_TKLOGS - 1;

  if( g_admin_tklog[ previous ] )
    count = g_admin_tklog[ previous ]->id + 1;

  if( g_admin_tklog[ admin_tklog_index ] )
    tklog = g_admin_tklog[ admin_tklog_index ];
  else
    tklog = G_Alloc( sizeof( g_admin_tklog_t ) );

  memset( tklog, 0, sizeof( g_admin_tklog_t ) );
  tklog->id = count;
  tklog->time = level.time - level.startTime;
  Q_strncpyz( tklog->name, attacker->client->pers.netname, sizeof( tklog->name ) );

  if( victim )
  {
    Q_strncpyz( tklog->victim, victim->client->pers.netname, sizeof( tklog->victim ) );
    tklog->damage = victim->client->tkcredits[ attacker->s.number ];
    tklog->value = victim->client->ps.stats[ STAT_MAX_HEALTH ];
  }

  tklog->team = attacker->client->ps.stats[ STAT_PTEAM ];
  if( meansOfDeath == MOD_GRENADE )
    tklog->weapon = WP_GRENADE;
  else if( tklog->team == PTE_HUMANS )
    tklog->weapon = attacker->s.weapon;
  else
    tklog->weapon = attacker->client->ps.stats[ STAT_PCLASS ];

  g_admin_tklog[ admin_tklog_index ] = tklog;
  admin_tklog_index++;
  if( admin_tklog_index >= MAX_ADMIN_TKLOGS )
    admin_tklog_index = 0;
}

qboolean G_admin_tklog( gentity_t *ent, int skiparg )
{
  g_admin_tklog_t *results[ 10 ];
  int result_index = 0;
  char *search_name = NULL;
  int index;
  int skip = 0;
  int skipped = 0;
  int checked = 0;
  char n1[ MAX_NAME_LENGTH ];
  char fmt_name[ 16 ];
  char argbuf[ 32 ];
  char *weaponName;
  int name_length = 10;
  int max_id = 0;
  int i;
  qboolean match;

  memset( results, 0, sizeof( results ) );

  index = admin_tklog_index;
  for( i = 0; i < 10; i++ )
  {
    int prev;

    prev = index - 1;
    if( prev < 0 )
      prev = MAX_ADMIN_TKLOGS - 1;
    if( !g_admin_tklog[ prev ] )
      break;
    if( g_admin_tklog[ prev ]->id > max_id )
      max_id = g_admin_tklog[ prev ]->id;
    index = prev;
  }

  if( G_SayArgc() > 1 + skiparg )
  {
    G_SayArgv( 1 + skiparg, argbuf, sizeof( argbuf ) );
    if( ( *argbuf >= '0' && *argbuf <= '9' ) || *argbuf == '-' )
    {
      int id;

      id = atoi( argbuf );
      if( id < 0 )
        id += ( max_id - 9 );
      else if( id <= max_id - MAX_ADMIN_TKLOGS )
        id = max_id - MAX_ADMIN_TKLOGS + 1;

      if( id + 9 >= max_id )
        id = max_id - 9;
      if( id < 1 )
        id = 1;
      for( i = 0; i < MAX_ADMIN_TKLOGS; i++ )
      {
        if( g_admin_tklog[ i ]->id == id )
        {
          index = i;
          break;
        }
      }
    }
    else
    {
      search_name = argbuf;
    }

    if( G_SayArgc() > 2 + skiparg && ( search_name ) )
    {
      char skipbuf[ 4 ];
      G_SayArgv( 2 + skiparg, skipbuf, sizeof( skipbuf ) );
      skip = atoi( skipbuf );
    }
  }

  if( search_name )
  {
    g_admin_tklog_t *result_swap[ 10 ];

    memset( &result_swap, 0, sizeof( result_swap ) );

    index = admin_tklog_index - 1;
    if( index < 0 )
      index = MAX_ADMIN_TKLOGS - 1;

    while( g_admin_tklog[ index ] &&
      checked < MAX_ADMIN_TKLOGS &&
      result_index < 10 )
    {
      match = qfalse;

      G_SanitiseString( g_admin_tklog[ index ]->name, n1, sizeof( n1 ) );
      if( strstr( n1, search_name ) )
        match = qtrue;

      if( match && skip > 0 )
      {
        match = qfalse;
        skip--;
        skipped++;
      }
      if( match )
      {
        result_swap[ result_index ] = g_admin_tklog[ index ];
        result_index++;
      }

      checked++;
      index--;
      if( index < 0 )
        index = MAX_ADMIN_TKLOGS - 1;
    }
    // search runs backwards, turn it around
    for( i = 0; i < result_index; i++ )
      results[ i ] = result_swap[ result_index - i - 1 ];
  }
  else
  {
    while( g_admin_tklog[ index ] && result_index < 10 )
    {
      results[ result_index ] = g_admin_tklog[ index ];
      result_index++;
      index++;
      if( index >= MAX_ADMIN_TKLOGS )
        index = 0;
    }
  }

  for( i = 0; results[ i ] && i < 10; i++ )
  {
    int l;

    G_DecolorString( results[ i ]->name, n1 );
    l = strlen( n1 );
    if( l > name_length )
      name_length = l;
  }
  ADMBP_begin( );
  for( i = 0; results[ i ] && i < 10; i++ )
  {
    int t;

    t = results[ i ]->time / 1000;

    G_DecolorString( results[ i ]->name, n1 );
    Com_sprintf( fmt_name, sizeof( fmt_name ), "%%%ds", 
      (int)( name_length + strlen( results[ i ]->name ) - strlen( n1 ) ) );
    Com_sprintf( n1, sizeof( n1 ), fmt_name, results[ i ]->name );

    if( results[ i ]->team == PTE_HUMANS )
      weaponName = BG_FindNameForWeapon( results[ i ]->weapon );
    else
      weaponName = BG_FindNameForClassNum( results[ i ]->weapon );

    ADMBP( va( "^7%3d %3d:%02d %s^7 %3d / %3d %10s %s^7\n",
      results[ i ]->id,
      t / 60, t % 60,
      n1,
      results[ i ]->damage,
      results[ i ]->value,
      weaponName,
      results[ i ]->victim ) );
  }
  if( search_name )
  {
    ADMBP( va( "^3!tklog:^7 Showing %d matches for '%s^7'.",
      result_index,
      argbuf ) );
    if( checked < MAX_ADMIN_TKLOGS && g_admin_tklog[ checked ] )
      ADMBP( va( " run '!tklog %s^7 %d' to see more",
       argbuf,
       skipped + result_index ) );
    ADMBP( "\n" );
  }
  else if ( results[ 0 ] )
  {
    ADMBP( va( "^3!tklog:^7 Showing %d - %d of %d.\n",
      results[ 0 ]->id,
      results[ result_index - 1 ]->id,
      max_id ) );
  }
  else
  {
    ADMBP( "^3!tklog:^7 log is empty.\n" );
  }
  ADMBP_end( );

  return qtrue;
}

void G_admin_maplog_update( void )
{
  char map[ 64 ];
  char maplog[ MAX_CVAR_VALUE_STRING ];
  char *ptr;
  int count = 0;

  trap_Cvar_VariableStringBuffer( "mapname", map, sizeof( map ) );

  Q_strncpyz( maplog, g_adminMapLog.string, sizeof( maplog ) );
  ptr = maplog;
  while( *ptr && count < MAX_ADMIN_MAPLOG_LENGTH ) 
  {
    while( *ptr != ' ' && *ptr != '\0' ) ptr++;

    count++;
    if( count >= MAX_ADMIN_MAPLOG_LENGTH )
    {
      *ptr = '\0';
    }

    if( *ptr == ' ' ) ptr++;
  }

  trap_Cvar_Set( "g_adminMapLog", va( "%s%s%s",
    map,
    ( maplog[0] != '\0'  ) ? " " : "",
    maplog ) );
}

void G_admin_maplog_result( char *flag )
{
  static int lastTime = 0;
  char maplog[ MAX_CVAR_VALUE_STRING ];
  int t;

  if( !flag )
    return;

  // avoid race when called in same frame
  if( level.time == lastTime )
    return;

  lastTime = level.time;

  if( g_adminMapLog.string[ 0 ] &&
    g_adminMapLog.string[ 1 ] == ';' )
  {
    // only one result allowed
    return;
  }

  if ( level.surrenderTeam != PTE_NONE )
  {
    if( flag[ 0 ] == 'a' )
    {
      if( level.surrenderTeam == PTE_HUMANS )
        flag = "A";
    }
    else if( flag[ 0 ] == 'h' )
    {
      if( level.surrenderTeam == PTE_ALIENS )
        flag = "H";
    }
  }

  t = ( level.time - level.startTime ) / 1000;
  Q_strncpyz( maplog, g_adminMapLog.string, sizeof( maplog ) );
  trap_Cvar_Set( "g_adminMapLog", va( "%1s;%03d:%02d;%s",
    flag,
    t / 60, t % 60,
    maplog ) );
}

qboolean G_admin_maplog( gentity_t *ent, int skiparg )
{
  char maplog[ MAX_CVAR_VALUE_STRING ];
  char *ptr;
  int count = 0;

  Q_strncpyz( maplog, g_adminMapLog.string, sizeof( maplog ) );

  ADMBP_begin( );
  ptr = maplog;
  while( *ptr != '\0' && count < MAX_ADMIN_MAPLOG_LENGTH + 1 )
  {
    char *end;
    const char *result = NULL;
    char *clock = NULL;
    char *colon;

    end = ptr;
    while( *end != ' ' && *end != '\0' ) end++;
    if( *end == ' ' )
    {
      *end = '\0';
      end++;
    }

    if( ptr[ 0 ] && ptr[ 1 ] == ';' )
    {
      switch( ptr[ 0 ] )
      {
        case 't':
          result = "^7tie";
          break;
        case 'a':
          result = "^1Alien win";
          break;
        case 'A':
          result = "^1Alien win ^7/ Humans admitted defeat";
          break;
        case 'h':
          result = "^4Human win";
          break;
        case 'H':
          result = "^4Human win ^7/ Aliens admitted defeat";
          break;
        case 'd':
          result = "^5draw vote";
          break;
        case 'N':
          result = "^6admin loaded next map";
          break;
        case 'r':
          result = "^2restart vote";
          break;
        case 'R':
          result = "^6admin restarted map";
          break;
        case 'm':
          result = "^2map vote";
          break;
        case 'l':
          result = "^2layout vote";
          break;
        case 'M':
          result = "^6admin changed map";
          break;
        case 'D':
          result = "^6admin loaded devmap";
          break;
        default:
          result = "";
          break;
      }
      ptr += 2;
      colon = strchr( ptr, ';' );
      if ( colon )
      {
        clock = ptr;
        ptr = colon + 1;
        *colon = '\0';

        // right justification with -6%s doesnt work..
        if( clock[ 0 ] == '0' && clock[ 1 ] != ':' )
        {
          if( clock[ 1 ] == '0' && clock[ 2 ] != ':' )
            clock[ 1 ] = ' ';
          clock[ 0 ] = ' ';
        }
      }
    }
    else if( count == 0 )
    {
      result = "^3current map";
      clock = "  -:--";
    }

    ADMBP( va( "%s%20s %-6s %s^7\n",
      ( count == 0 ) ? "^3" : "^7",
      ptr,
      ( clock ) ? clock : "",
      ( result ) ? result : "" ) );

    ptr = end;
    count++;
  }

  if( g_nextMap.string[0] )
  {
    ADMBP( va( "^5NextMap override: %s\n", g_nextMap.string ) );
  }

  ADMBP_end( );

  return qtrue;
}

#define MAX_LISTMAPS_MAPS 256

static int SortMaps(const void *a, const void *b)
{
  return strcmp(*(char **)a, *(char **)b);
}

qboolean G_admin_listmaps( gentity_t *ent, int skiparg )
{
  char fileList[ 4096 ] = {""};
  char *fileSort[ MAX_LISTMAPS_MAPS ];
  char search[ 16 ] = {""};
  int numFiles;
  int i;
  int fileLen = 0;
  int  count = 0;
  char *filePtr;
  int rows;

  if( G_SayArgc( ) > 1 + skiparg )
  {
    G_SayArgv( skiparg + 1, search, sizeof( search ) );
  }

  numFiles = trap_FS_GetFileList( "maps/", ".bsp",
    fileList, sizeof( fileList ) );
  filePtr = fileList;
  for( i = 0; i < numFiles && count < MAX_LISTMAPS_MAPS; i++, filePtr += fileLen + 1 )
  {
    fileLen = strlen( filePtr );
    if (fileLen < 5)
      continue;

    filePtr[ fileLen - 4 ] = '\0';

    if( search[ 0 ] && !strstr( filePtr, search ) )
      continue;

    fileSort[ count ] = filePtr;
    count++;
  }

  qsort(fileSort, count, sizeof(fileSort[ 0 ]), SortMaps);

  rows = count / 3;
  if ( rows * 3 < count ) rows++;

  ADMBP_begin();
  for( i = 0; i < rows; i++ )
  {
    ADMBP( va( "^7%20s %20s %20s\n",
      fileSort[ i ],
      ( rows + i < count ) ? fileSort[ rows + i ] : "",
      ( rows * 2 + i < count ) ? fileSort[ rows * 2 + i ] : "" ) );
  }
  if ( search[ 0 ] )
    ADMBP( va( "^3!listmaps: ^7found %d maps matching '%s^7'.\n", count, search ) );
  else
    ADMBP( va( "^3!listmaps: ^7listing %d maps.\n", count ) );

  ADMBP_end();

  return qtrue;
}

static int SortFlags( const void *pa, const void *pb )
{
  char *a = (char *)pa;
  char *b = (char *)pb;

  if( *a == '-' || *a == '+' )
    a++;
  if( *b == '-' || *b == '+' )
    b++;
  return strcmp(a, b);
}

#define MAX_USER_FLAGS 200
const char *G_admin_user_flag( char *oldflags, char *flag, qboolean add, qboolean clear,
                               char *newflags, int size )
{
  char *token, *token_p;
  char *key;
  char head_flags[ MAX_USER_FLAGS ][ MAX_ADMIN_FLAG_LEN ];
  char tail_flags[ MAX_USER_FLAGS ][ MAX_ADMIN_FLAG_LEN ];
  char allflag[ MAX_ADMIN_FLAG_LEN ];
  char newflag[ MAX_ADMIN_FLAG_LEN ];
  int head_count = 0;
  int tail_count = 0;
  qboolean flagset = qfalse;
  int i;

  if( !flag[ 0 ] )
  {
    return "invalid admin flag";
  }

  allflag[ 0 ] = '\0';
  token_p = oldflags;
  while( *( token = COM_Parse( &token_p ) ) )
  {
    key = token;
    if( *key == '-' || *key == '+' )
      key++;

    if( !strcmp( key, flag ) )
    {
      if( flagset )
        continue;
      flagset = qtrue;
      if( clear )
      {
        // clearing ALLFLAGS will result in clearing any following flags
        if( !strcmp( key, ADMF_ALLFLAGS ) )
          break;
        else
          continue;
      }
      Com_sprintf( newflag, sizeof( newflag ), "%s%s",
        ( add ) ? "+" : "-", key );
    }
    else
    {
      Q_strncpyz( newflag, token, sizeof( newflag ) );
    }

    if( !strcmp( key, ADMF_ALLFLAGS ) )
    {
      if( !allflag[ 0 ] )
        Q_strncpyz( allflag, newflag, sizeof( allflag ) );
      continue;
    }

    if( !allflag[ 0 ] )
    {
      if( head_count < MAX_USER_FLAGS )
      {
        Q_strncpyz( head_flags[ head_count ], newflag,
                    sizeof( head_flags[ head_count ] ) );
        head_count++;
      }
    }
    else
    {
      if( tail_count < MAX_USER_FLAGS )
      {
        Q_strncpyz( tail_flags[ tail_count ], newflag,
                    sizeof( tail_flags[ tail_count ] ) );
        tail_count++;
      }
    }
  }

  if( !flagset && !clear )
  {
    if( !strcmp( flag, ADMF_ALLFLAGS ) )
    {
      Com_sprintf( allflag, sizeof( allflag ), "%s%s",
        ( add ) ? "" : "-", ADMF_ALLFLAGS );
    }
    else if( !allflag[ 0 ] )
    {
      if( head_count < MAX_USER_FLAGS )
      {
        Com_sprintf( head_flags[ head_count ], sizeof( head_flags[ head_count ] ),
          "%s%s", ( add ) ? "" : "-", flag );
        head_count++;
      }
    }
    else
    {
      if( tail_count < MAX_USER_FLAGS )
      {
        Com_sprintf( tail_flags[ tail_count ], sizeof( tail_flags[ tail_count ] ),
          "%s%s", ( add ) ? "+" : "-", flag );
        tail_count++;
      }
    }
  }

  qsort( head_flags, head_count, sizeof( head_flags[ 0 ] ), SortFlags );
  qsort( tail_flags, tail_count, sizeof( tail_flags[ 0 ] ), SortFlags );

  // rebuild flags
  newflags[ 0 ] = '\0';
  for( i = 0; i < head_count; i++ )
  {
    Q_strcat( newflags, size,
              va( "%s%s", ( i ) ? " ": "", head_flags[ i ] ) );
  }
  if( allflag[ 0 ] )
  {
    Q_strcat( newflags, size,
      va( "%s%s", ( newflags[ 0 ] ) ? " ": "", allflag ) );

    for( i = 0; i < tail_count; i++ )
    {
      Q_strcat( newflags, size,
                va( " %s", tail_flags[ i ] ) );
    }
  }

  return NULL;
}

typedef struct {
  char *flag;
  char *description;
} AdminFlagListEntry_t;

static AdminFlagListEntry_t adminFlagList[] =
{
  { ADMF_ACTIVITY,             "inactivity rules do not apply" },
  { ADMF_ADMINCHAT,            "can see and use admin chat" },
  { ADMF_ALLFLAGS,             "has all flags and can use any command" },
  { ADMF_BAN_IMMUNITY,         "immune from IP bans" },
  { ADMF_CAN_PERM_BAN,         "can permanently ban players" },
  { ADMF_DBUILDER,             "permanent designated builder" },
  { ADMF_FORCETEAMCHANGE,      "team balance rules do not apply" },
  { ADMF_INCOGNITO,            "does not show as admin in !listplayers" },
  { ADMF_IMMUNITY,             "cannot be vote kicked or muted" },
  { ADMF_IMMUTABLE,            "admin commands cannot be used on them" },
  { ADMF_NOCENSORFLOOD,        "no flood protection" },
  { ADMF_NO_VOTE_LIMIT,        "vote limitations do not apply" },
  { ADMF_SEESFULLLISTPLAYERS,  "sees all info in !listplayers" },
  { ADMF_SPEC_ALLCHAT,         "can see team chat as spectator" },
  { ADMF_ADMINSTEALTH,         "uses admin stealth" },
  { ADMF_TEAMCHANGEFREE,       "keeps credits on team switch" },
  { ADMF_TEAMCHAT_CMD,         "can run commands from team chat" },
  { ADMF_UNACCOUNTABLE,        "does not need to specify reason for kick/ban" }
//  { ADMF_NO_CHAT,              "can not talk" },
//  { ADMF_NO_VOTE,              "can not call votes" }
};
static int adminNumFlags= sizeof( adminFlagList ) / sizeof( adminFlagList[ 0 ] );

#define MAX_LISTCOMMANDS 128
qboolean G_admin_flaglist( gentity_t *ent, int skiparg )
{
  qboolean shown[ MAX_LISTCOMMANDS ];
  int i, j;
  int count = 0;

  ADMBP_begin();

  ADMBP( "^3Ability flags:\n" );

  for( i = 0; i < adminNumFlags; i++ )
  {
    ADMBP( va( "  %s%-20s ^7%s\n",
      ( adminFlagList[ i ].flag[ 0 ] != '.' ) ? "^5" : "^1",
      adminFlagList[ i ].flag,
      adminFlagList[ i ].description ) );
  }

  ADMBP( "^3Command flags:\n" );

  memset( shown, 0, sizeof( shown ) );
  for( i = 0; i < adminNumCmds; i++ )
  {
    if( i < MAX_LISTCOMMANDS && shown[ i ] )
      continue;
    ADMBP( va( "  ^5%-20s^7", g_admin_cmds[ i ].flag ) );
    for( j = i; j < adminNumCmds; j++ )
    {
      if( !strcmp( g_admin_cmds[ j ].flag, g_admin_cmds[ i ].flag ) )
      {
        ADMBP( va( " %s", g_admin_cmds[ j ].keyword ) );
        if( j < MAX_LISTCOMMANDS )
          shown[ j ] = qtrue;
      }
    }
    ADMBP( "\n" );
    count++;
  }

  ADMBP( va( "^3!flaglist: ^7listed %d abilities and %d command flags\n",
    adminNumFlags, count ) );

  ADMBP_end();

  return qtrue;
}

qboolean G_admin_flag( gentity_t *ent, int skiparg )
{
  char command[ MAX_ADMIN_CMD_LEN ], *cmd;
  char name[ MAX_NAME_LENGTH ];
  char flagbuf[ MAX_ADMIN_FLAG_LEN ];
  char *flag;
  int id;
  char adminname[ MAX_NAME_LENGTH ] = {""};
  const char *result;
  qboolean add = qtrue;
  qboolean clear = qfalse;
  int admin_level = -1;
  int i, level;

  G_SayArgv( skiparg, command, sizeof( command ) );
  cmd = command;
  if( *cmd == '!' )
    cmd++;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( va( "^3!%s: ^7usage: !%s slot# flag\n", cmd, cmd ) );
    return qfalse;
  }

  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( name[ 0 ] == '*' )
  {
    if( ent )
    {
      ADMP( va( "^3!%s: only console can change admin level flags\n", cmd ) );
      return qfalse;
    }
    id = atoi( name + 1 );
    for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
    {
      if( g_admin_levels[ i ]->level == id )
      {
        admin_level = i;
        break;
      }
    }
    if( admin_level < 0 )
    {
      ADMP( va( "^3!%s: admin level %d does not exist\n", cmd, id ) );
      return qfalse;
    }
    Com_sprintf( adminname, sizeof( adminname ), "admin level %d", id );
  }
  else
  {
    id = G_admin_find_admin_slot( ent, name, cmd, adminname, sizeof( adminname ) );
    if( id < 0 )
      return qfalse;

    if( ent && !admin_higher_guid( ent->client->pers.guid, g_admin_admins[ id ]->guid ) )
    {
      ADMP( va( "^3%s:^7 your intended victim has a higher admin level than you\n", cmd ) );
      return qfalse;
    }
  }

  if( G_SayArgc() < 3 + skiparg )
  {
    flag = "";
    level = 0;
    if( admin_level < 0 )
    {
      for( i = 0; i < MAX_ADMIN_LEVELS && g_admin_levels[ i ]; i++ )
      {
        if( g_admin_admins[ id ]->level == g_admin_levels[ i ]->level )
        {
          flag = g_admin_levels[ i ]->flags;
          level = g_admin_admins[ id ]->level;
          break;
        }
      }
      ADMP( va( "^3%s:^7 flags for %s^7 are '^3%s^7'\n",
        cmd, adminname, g_admin_admins[ id ]->flags) );
    }
    else
    {
      flag = g_admin_levels[ admin_level ]->flags;
      level = g_admin_levels[ admin_level ]->level;
    }
    ADMP( va( "^3%s:^7  level %d flags are '%s'\n",
      cmd, level, flag ) );

    return qtrue;
  }

  G_SayArgv( 2 + skiparg, flagbuf, sizeof( flagbuf ) );
  flag = flagbuf;
  if( flag[ 0 ] == '-' || flag[ 0 ] == '+' )
  {
    add = ( flag[ 0 ] == '+' );
    flag++;
  }
  if( ent && !Q_stricmp( ent->client->pers.guid, g_admin_admins[ id ]->guid ) )
  {
    ADMP( va( "^3%s:^7 you may not change your own flags (use rcon)\n", cmd ) );
    return qfalse;
  }
  if( flag[ 0 ] != '.' && !G_admin_permission( ent, flag ) )
  {
    ADMP( va( "^3%s:^7 you can only change flags that you also have\n", cmd ) );
    return qfalse;
  }

  if( !Q_stricmp( cmd, "unflag" ) )
  {
    clear = qtrue;
  }

  if( admin_level < 0 )
  {
    result = G_admin_user_flag( g_admin_admins[ id ]->flags, flag, add, clear,
                                g_admin_admins[ id ]->flags, sizeof( g_admin_admins[ id ]->flags ) );
  }
  else
  {
    result = G_admin_user_flag( g_admin_levels[ admin_level ]->flags, flag, add, clear,
                                g_admin_levels[ admin_level ]->flags,
                                sizeof( g_admin_levels[ admin_level ]->flags ) );
  }
  if( result )
  {
    ADMP( va( "^3!flag: ^7an error occured setting flag '^3%s^7', %s\n",
      flag, result ) );
    return qfalse;
  }

  if( !Q_stricmp( cmd, "flag" ) )
  {
    G_AdminsPrintf( "^3!%s: ^7%s^7 was %s admin flag '%s' by %s\n",
      cmd, adminname,
      ( add ) ? "given" : "denied",
      flag,
      ( ent ) ? ent->client->pers.netname : "console" );
  }
  else
  {
    G_AdminsPrintf( "^3!%s: ^7admin flag '%s' for %s^7 cleared by %s\n",
      cmd, flag, adminname,
      ( ent ) ? ent->client->pers.netname : "console" );
  }

  if( !g_admin.string[ 0 ] )
    ADMP( va( "^3!%s: ^7WARNING g_admin not set, not saving admin record "
      "to a file\n", cmd ) );
  else
    G_admin_writeconfig( );

  return qtrue;
}

qboolean G_admin_suspendban( gentity_t *ent, int skiparg )
{
  int bnum;
  int length;
  int expires = 0;
  char *arg;
  char bs[ 5 ];
  char duration[ 32 ];

  if( G_SayArgc() < 3 + skiparg )
  {
    ADMP( "^3!suspendban: ^7usage: !suspendban [ban #] [length]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, bs, sizeof( bs ) );
  bnum = atoi( bs );
  if( bnum < 1 || !g_admin_bans[ bnum - 1] )
  {
    ADMP( "^3!suspendban: ^7invalid ban #\n" );
    return qfalse;
  }

  arg = G_SayConcatArgs( 2 + skiparg );
  length = G_admin_parse_time( arg );
  if( length < 0 )
  {
    ADMP( "^3!suspendban: ^7invalid length\n" );
    return qfalse;
  }
  if( length > MAX_ADMIN_BANSUSPEND_DAYS * 24 * 60 * 60 )
  {
    length = MAX_ADMIN_BANSUSPEND_DAYS * 24 * 60 * 60;
    ADMP( va( "^3!suspendban: ^7maximum ban suspension is %d days\n",
      MAX_ADMIN_BANSUSPEND_DAYS ) );
  }

  if ( length > 0 )
  {
    qtime_t qt;

    expires = trap_RealTime( &qt ) + length;
  }
  if( g_admin_bans[ bnum - 1 ]->suspend == expires )
  {
    ADMP( "^3!suspendban: ^7no change\n" );
    return qfalse;
  }

  g_admin_bans[ bnum - 1 ]->suspend = expires;
  if ( length > 0 )
  {
    G_admin_duration( length , duration, sizeof( duration ) );
    AP( va( "print \"^3!suspendban: ^7ban #%d suspended for %s\n\"",
      bnum, duration ) );
  }
  else
  {
    AP( va( "print \"^3!suspendban: ^7ban #%d suspension removed\n\"",
      bnum ) );
  }

  if( !g_admin.string[ 0 ] )
    ADMP( "^3!adjustban: ^7WARNING g_admin not set, not saving ban to a file\n" );
  else
    G_admin_writeconfig( );
  return qtrue;
}

qboolean G_admin_demo( gentity_t *ent, int skiparg )
{
  if( !ent )
  {
    ADMP( "!demo: console can not use demo.\n");
    return qfalse;
  }

  ent->client->pers.ignoreAdminWarnings = !( ent->client->pers.ignoreAdminWarnings );

  ADMP( va( "^3!demo: ^7your visibility of admin chat is now %s\n",
    ( ent->client->pers.ignoreAdminWarnings ) ? "^1disabled" : "^2enabled" ) );

  return qtrue;
}

qboolean G_admin_nobuild( gentity_t *ent, int skiparg )
{
  int minargc;
  char units[ MAX_STRING_CHARS ];
  char units2[ MAX_STRING_CHARS ];
  int units3;
  int units4;
  minargc = 3 + skiparg;
  
  if( !ent )
  {
    ADMP( "^3!nobuild: ^7console cannot use this command\n" );
    return qfalse;
  }
  
  G_SayArgv( 1 + skiparg, units, sizeof( units ) );
  G_SayArgv( 2 + skiparg, units2, sizeof( units2 ) );
 
  units3 = atoi(units);
  units4 = atoi(units2);

  if( level.nobuild == qfalse )
  {
    if( G_SayArgc() < minargc )
    {
        ADMP( "^3!nobuild: ^7usage: !nobuild [game units area] [gane units height]\n" );
        return qfalse;
    }

    level.nobuild = qtrue;
    level.nobuildArea = units3;
    level.nobuildHeight = units4;
    AP( va( "print \"^3!nobuild: ^7nobuild mode has been enabled by %s^7\n\"", ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  else
  {
    level.nobuild = qfalse;
    level.nobuildArea = 0.0f;
    AP( va( "print \"^3!nobuild: ^7nobuild mode has been disabled by %s^7\n\"", ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  
  return qtrue;
}

qboolean G_admin_nobuildsave( gentity_t *ent, int skiparg )
{

  trap_SendConsoleCommand( EXEC_APPEND, "nobuildsave" );
  AP( va( "print \"^3!nobuildsave: ^7nobuild markers saved by %s\n\"", ( ent ) ? ent->client->pers.netname : "console" ) );
  
  return qtrue;
}

qboolean G_admin_lockname( gentity_t *ent, int skiparg )
{
  int pids[ MAX_CLIENTS ];
  char name[ MAX_NAME_LENGTH ], err[ MAX_STRING_CHARS ];
  gentity_t *vic;

  if( G_SayArgc() < 2 + skiparg )
  {
    ADMP( "^3!lockname: ^7usage: !lockname [name|slot#]\n" );
    return qfalse;
  }
  G_SayArgv( 1 + skiparg, name, sizeof( name ) );
  if( G_ClientNumbersFromString( name, pids ) != 1 )
  {
    G_MatchOnePlayer( pids, err, sizeof( err ) );
    ADMP( va( "^3!lockname: ^7%s\n", err ) );
    return qfalse;
  }
  if( !admin_higher( ent, &g_entities[ pids[ 0 ] ] ) )
  {
    ADMP( "^3!lockname: ^7sorry, but your intended victim has a higher admin"
        " level than you\n" );
    return qfalse;
  }
  vic = &g_entities[ pids[ 0 ] ];
  if( vic->client->pers.nlocked == qtrue )
  {
    vic->client->pers.nlocked = qfalse;
    CPx( pids[ 0 ], "cp \"^1Your name has been unlocked\"" );
    AP( va( "print \"^3!lockname: ^7%s^7's name has been unlocked by %s\n\"",
            vic->client->pers.netname,
            ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  else
  {
    vic->client->pers.nlocked = qtrue;
    CPx( pids[ 0 ], "cp \"^1Your name has been locked\"" );
    AP( va( "print \"^3!lockname: ^7%s^7's name has been locked by ^7%s\n\"",
            vic->client->pers.netname,
            ( ent ) ? ent->client->pers.netname : "console" ) );
  }
  ClientUserinfoChanged( pids[ 0 ] );
  return qtrue;
}

qboolean G_admin_invisible( gentity_t *ent, int skiparg )
{
  if( !ent )
  {
    ADMP( "!invisible: console can not become invisible.\n" );
    return qfalse;
  }
  // Only allow spectators to become invisible
  if ( ent->client->sess.sessionTeam != TEAM_SPECTATOR )
  {
    ADMP( "^3!invisible: ^7only spectators can become invisible\n" );
    return qfalse;
  }

  if ( ent->client->sess.invisible != qtrue )
  {
    // Make the player invisible
    ent->client->sess.invisible = qtrue;
    ClientUserinfoChanged( ent->client->pers.connection->clientNum );
    G_admin_namelog_update( ent->client, qtrue );
    trap_SendServerCommand( -1, va( "print \"%s" S_COLOR_WHITE " disconnected\n\"", ent->client->pers.netname ) );
  }
  else
  {
    // Make the player visible
    ent->client->sess.invisible = qfalse;
    ClientUserinfoChanged( ent->client->pers.connection->clientNum );
    G_admin_namelog_update( ent->client, qfalse );
    trap_SendServerCommand( -1, va( "print \"%s" S_COLOR_WHITE " connected\n\"", ent->client->pers.netname ) );
    trap_SendServerCommand( -1, va( "print \"%s" S_COLOR_WHITE " entered the game\n\"", ent->client->pers.netname ) );
  }
  return qtrue;
}

typedef struct {
    int allowVote;
    int minLevelMM1;
    int privMsg;
    char motd[ MAX_STRING_CHARS ];
} ScrimmSavedValues_t;

static ScrimmSavedValues_t ssv;

void G_admin_scrimm_switch( qboolean state )
{
    level.scrimmMode = state;

    level.alienTeamLocked = state;
    level.humanTeamLocked = state;

    if( state )
    {
        ssv.allowVote = g_allowVote.integer;
        ssv.minLevelMM1 = g_minLevelToSpecMM1.integer;
        ssv.privMsg = g_privateMessages.integer;

        trap_Cvar_Set( "g_allowVote", "0" );
        trap_Cvar_Set( "g_minLevelToSpecMM1", "5" );
        trap_Cvar_Set( "g_privateMessages", "0" );

        if( g_scrimmMotd.string[ 0 ] )
        {
            trap_Cvar_VariableStringBuffer( "g_motd", ssv.motd, sizeof( ssv.motd ) );
            trap_Cvar_Set( "g_motd", g_scrimmMotd.string );
            trap_SetConfigstring( CS_MOTD, g_scrimmMotd.string );
        }
    }
    else
    {
        trap_Cvar_Set( "g_allowVote", va("%i", ssv.allowVote) );
        trap_Cvar_Set( "g_minLevelToSpecMM1", va("%i", ssv.minLevelMM1) );
        trap_Cvar_Set( "g_privateMessages", va("%i", ssv.privMsg) );

        if( g_scrimmMotd.string[ 0 ] )
        {
            trap_Cvar_Set( "g_motd", ssv.motd );
            trap_SetConfigstring( CS_MOTD, ssv.motd );
        }
    }
}

qboolean G_admin_scrimm( gentity_t *ent, int skiparg )
{
    char s[ 4 ];

    if( G_SayArgc( ) < 2 + skiparg )
    {
        ADMP( "^3!scrimm: ^7usage: !scrimm [on|off]\n" );
        return qfalse;
    }

    G_SayArgv( 1 + skiparg, s, sizeof( s ) );

    if( !Q_stricmp( s, "on" ) )
    {
        if( level.scrimmMode )
        {
            ADMP( "^3!scrimm: ^7Scrimm mode is already ON\n" );
            return qfalse;
        }
        trap_SendServerCommand( -1, "cp \"^3SCRIMM ^7mode is now ^2ON\"");
        AP( va( "print \"^3!scrimm: ^7Scrimm mode turned on by %s\n\"",
          ( ent ) ? ent->client->pers.netname : "console" ) );
        G_admin_scrimm_switch( qtrue );
        return qtrue;

    }
    else if ( !Q_stricmp( s, "off" ) )
    {
        if( !level.scrimmMode )
        {
            ADMP( "^3!scrimm: ^7Scrimm mode is already OFF\n" );
            return qfalse;
        }
        trap_SendServerCommand( -1, "cp \"^3SCRIMM ^7mode is now ^1OFF\"");
        AP( va( "print \"^3!scrimm: ^7Scrimm mode turned off by %s\n\"",
          ( ent ) ? ent->client->pers.netname : "console" ) );
        G_admin_scrimm_switch( qfalse );
        return qtrue;
    }
    else
    {
        ADMP( "^3!scrimm: ^7Invalid option. Options are on/off.\n" );
        return qfalse;
    }
}

qboolean G_admin_stage( gentity_t *ent, int skiparg )
{
    char team[ 2 ];
    char stage_arg[ 2 ];
    int stage = 0;

    if( G_SayArgc( ) < 3 + skiparg )
    {
        ADMP( "^3!stage: ^7usage: !stage [a|h] [1|2|3]\n" );
        return qfalse;
    }
    G_SayArgv( 1 + skiparg, team, sizeof( team ) );
    G_SayArgv( 2 + skiparg, stage_arg, sizeof( stage_arg ) );

    stage = atoi( stage_arg );

    if( ( Q_stricmp("a", team) && Q_stricmp("h", team) ) || stage < S1+1 || stage > S3+1)
    {
        ADMP( "^3!stage: ^7invalid team or stage\n" );
        return qfalse;
    }

    if( !Q_stricmp("a", team) )
    {
        trap_Cvar_Set( "g_alienStage", va("%i", stage-1) );
        AP( va("print \"^3!stage: ^7Aliens are now stage %i\n\"", stage) );
        return qtrue;
    }
    if( !Q_stricmp("h", team) )
    {
        trap_Cvar_Set( "g_humanStage", va("%i", stage-1) );
        AP( va("print \"^3!stage: ^7Humans are now stage %i\n\"", stage) );
        return qtrue;
    }
    return qtrue;
}
