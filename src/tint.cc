#include <string>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>

#include <wire/wire.hpp>
#include <heal/heal.hpp>

#include "console-colors.h"
#include "console-colors.c"
#include "logger.hpp"

#pragma comment(lib,"user32.lib")

#ifdef _MSC_VER
#define popen _popen
#define pclose _pclose
#endif

void ColorPrintf(int color, const char* fmt, ...);


bool getline( FILE *fp, std::string &k ) {
  k.clear();
  while( fp ) {
    int ch = fgetc(fp);
    if( ch == EOF ) return false;
    k += ch;
    if( ch == '\r' || ch == '\n' ) return true;
  }
  return false;
}

bool streamserver( FILE *fp ) {
    wire::string line, tag;
    int open = 0;
    int fg = 0, bg = 0;

    while( getline(fp, line) ) {
				auto split = line.split("[;m\x1b");
				while( !split.empty() ) {
          auto tok = *split.begin();
          split.pop_front(); // esc
          if( tok != "\x1b" ) {
              cc_fprintf( (CC_FG_NONE+fg) | (CC_BG_NONE+bg), stdout, "%s", tok.c_str() );
//              ColorPrintf( fg, "%s", tok.c_str() );
              html::io << tok;
              continue;
          } else {
               tok = *split.begin();
               split.pop_front(); // esc
               if( tok != "[" ) {
                 cc_fprintf( (CC_FG_NONE+fg) | (CC_BG_NONE+bg), stdout, "%s", "\x1b[" );
//                 ColorPrintf( fg, "%s", "\x1b[" );
                 html::io << "\x1b";
                 continue;
               }
          }

          auto &wd = split;
          while( wd.size() ) {

            wire::string w( wd.front() );
            wd.pop_front();

            /**/ if( w == ";" ) continue;
            else if( w == "m" ) break;

            int code = w.as<int>();

            /**/ if( code ==  39 ) fg = 0;
            else if( code ==  49 ) bg = 0;
            else if( code >=  30 && code <=  37 ) fg = ( code -  30 ) + 1;
            else if( code >=  40 && code <=  47 ) bg = ( code -  40 ) + 1;
            else if( code >=  90 && code <=  99 ) fg = ( code -  90 ) + 1 + 8;
            else if( code >= 100 && code <= 109 ) bg = ( code - 100 ) + 1 + 8;
            else { std::cout << code << " :(\n";
              html::io << code << " :(\n";
            }
          }
				}
    }

    return true;
}

bool fserver( std::string &cmdline ) {
   FILE *fp = popen( cmdline.c_str(), "r" );
   if( !fp ) {
    return false;
   }
   if( !streamserver(fp) ) {
    return false;
   }
   pclose(fp);
   return true;
}

int main( int argc, const char **argv ) {

  //Log(0, "this is a test", __FILE__, 'h', __LINE__);

  html::set_highlights( {
    "err", "warn", "log", "info",
    "mesh", "texture", "asset",
    "load", "save", "init",
    "debug", "release"
  } );

  /*
  SetConsoleMode(
  _In_  HANDLE hConsoleHandle,
  ENABLE_EXTENDED_FLAGS ); */

  if( argc > 1 ) {
      std::string args;
      for( int i = 1; i < argc; ++i ) {
          args += std::string() + argv[i] + " ";
      }
      //args += std::string() + "|" + argv[0];
      //return system( args.c_str() );
      fserver(args + " 2>&1");
      cc_restore( stdout );
      return 0;
  } else {
      std::cout << "tint: nothing to pipe" << std::endl;
      return 0;
  }
}
