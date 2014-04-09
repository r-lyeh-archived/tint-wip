// ajax html logger. not so simple. mit licensed.
// - rlyeh

// original jscript logger by Herman Tulleken
// ref [1]: http://devmag.org.za/2011/01/25/make-your-logs-interactive-and-squash-bugs-faster/

#include <iostream>

#include <sao/sao.hpp>
#include <sao/file.hpp>
#include <dot/dot.hpp>
#include <wire/wire.hpp>
#include <sand/sand.hpp>
#include <heal/heal.hpp>

namespace console
{
    wire::strings log;

    void custom_logger( bool open, bool feed, bool close, const std::string &line )
    {
        if( open )
        {}
        else
        if( close )
        {}
        else
        if( feed )
        {
            log.push_back( std::string() );

            while( log.size() > 25 ) log.pop_front();

            fprintf( stdout, "\n" );
        }
        else
        {
            if( log.size() )
                log.back() += line;
            else
                log.push_back( line );

            fprintf( stdout, "%s", line.c_str() );
        }
    }
}

namespace html
{
    enum { resolve_stack = false };

    namespace
    {
        //const std::string filename = app::get_name() + ".html";
        const std::string indexname = ".tint.html";
        const std::string filename = sand::format( sand::now(), ".tint.%Y%m%d-%H%M%S.ajax" );

        std::string get_hue_color( int slice, int max )
        {
            static float hue_rand_step = rand() / float(RAND_MAX);
            float hue = slice * ( 1.f / max ) + hue_rand_step;
            dot::pixel p = dot::pixel::hsla( hue <= 1.f ? hue : hue - 1.f, 0.5f, 0.5f, 1.f ).to_rgba();
            return wire::format("#%02x%02x%02x", int(p.r * 255.f), int(p.g * 255.f), int(p.b * 255.f) );
        }
    }

    void readme()
    {

        /*
        system( wire::string("start
            "%LOCALAPPDATA%"\\Google\\Chrome\\Application\\chrome.exe "http://restart" --allow-file-access-from-files\n
        system( wire::string("start
            "%ProgramFiles(x86)%"\\Google\\Chrome\\Application\\chrome.exe "http://restart" --allow-file-access-from-files\n
        system( wire::string("start "%ProgramFiles%"\\Google\\Chrome\\Application\\chrome.exe "http://restart" --allow-file-access-from-files\n


        --disable-web-security --user-agent="Android" --user-data-dir="%TEMP%" --app="file:///C:/prj/tint/src/.tint.20140410-135734.html"
        */
        system( wire::string("start \1", indexname).c_str() );
    }

    //static const bool onexit = (std::atexit(readme), true);

    wire::strings get_default_highlights() {
        return std::vector< std::string >( {"err", "warn", "info", "log" });
    }

    void set_highlights( const std::vector< std::string > &_highlights ) {
        std::string colors;
        wire::strings highlights = _highlights;
        for( size_t i = 0; i < highlights.size(); ++i )
        {
            /*
                ".cassert {\n"
                "color:#e48625;\n"
                "background-color:#ffeecc;\n"
                "font-weight:bold;\n"
                "}\n"
            */
            highlights[i] = highlights[i].lowercase();
            colors += wire::string(".class_\1 { background-color: \2 }\n", highlights[i], get_hue_color(i, highlights.size()));
        }
        sao::file( ".tint.colors.css" ).overwrite( colors );

        wire::string code =
        highlights.str("<input type=\"button\" value=\"\1\" class=\"class_\1 Button\" onclick=\"hide_class('class_\1')\"/>\n");

/*
        code = std::string() +
        "<div id='Header'>" + code + "</div>"
        "<script>"
          "var me = document.currentScript.ownerDocument;"
          "var post = me.querySelector('#Header');"
          "var container = document.querySelector('#Header2');"
          "container.appendChild(post.cloneNode(true));"
        "</script>"; */

        sao::file( ".tint.interface.ajax" ).overwrite( code );
    }

    void custom_logger( bool open, bool feed, bool close, const std::string &line )
    {
        static wire::string cache;
        static sand::dt dt;

        static bool once = true;

        if( open && once )
        {
            // we use same logger for std::cout, cerr and clog...
            // and we dont want second and third loggers to overwrite first log
            once = false;

            const std::string jscript =
                "/* Functions for toggling visibility in the log. */\n"
                "// Gets all the elements in the node of the given class and tag.\n"
                "function getElementsByClass(searchClass,node,tag) {\n"
                        "var classElements = new Array();\n"
                        "if ( node == null ) node = document;\n"
                        "if ( tag == null ) tag = '*';\n"
                        "var elements = node.getElementsByTagName(tag);\n"
                        "var elementsSize = elements.length;\n"
                        "var pattern = new RegExp(\"(^|\\\\s)\" + searchClass + \"(\\\\s|$)\");\n"
                        "for (i = 0, j = 0; i < elementsSize; i++) {\n"
                                "if ( pattern.test(elements[i].className) ) {\n"
                                        "classElements[j] = elements[i];\n"
                                        "j++;\n"
                        "}}\n"
                        "return classElements;\n"
                "}\n"
                "// Toggles visibility of stacks in the HTML log.\n"
                "function hide(id) {\n"
                        "var element_style = document.getElementById(id).style;\n"
                        "var status = element_style.display;\n"
                        "if (status != 'block') element_style.display = 'block';\n"
                        "else element_style.display = 'none'; //status == visible\n"
                "}\n"
                "// Toggles visibility of a class in the HTML log.\n"
                "function hide_class(className) {\n"
                        "var elements = getElementsByClass(className);\n"
                        "var pattern = new RegExp(\"(^|\\\\s)Button(\\\\s|$)\");\n"
                        "for(i = 0; i < elements.length; i++) {\n"
                                "if(!pattern.test(elements[i].className)) {\n"
                                        "if(elements[i].style.display != 'none') elements[i].style.display = 'none'\n"
                                        "else elements[i].style.display = 'block'\n"
                "}}}\n";

            wire::string css(
                    "pre {\n"
                    "    display:none;\n"
                    "}\n"
                    "a {\n"
                    "    color:#ff0000;\n"
                    "    background-color:#ffffff;\n"
                    "    padding:3px;\n"
                    "    font-size:small;\n"
                    "    text-decoration: none;\n"
                    "    width:10em;\n"
                    "    font-weight:normal;\n"
                    "    cursor: hand;\n"
                    "    border-radius:4px;\n"
                    "}\n"
                    "h1 {\n"
                        "padding-top: 3em;\n"
                    "}\n"
                    "body {\n"
                        "font-family:\"Calibri\";\n"
                        "padding-top: 0;\n"
                        "margin-top: 0;\n"
                    "}\n"
                    "p {\n"
                        "font-family:\"Calibri\";\n"
                        "padding:4px;\n"
                        "border-radius:5px;\n"
                        "margin:2px;\n"
                        "display:block;\n"
                    "}\n"
                    "img {\n"
                        "float: none;\n"
                    "}\n"
                    ".Header {\n"
                        "position:fixed;\n"
                        "background:#ffffff;\n"
                        "padding-bottom:1em;\n"
                    "}\n"
                    ".Padding {\n"
                        "height:20em;\n"
                    "}\n"
                    "\n"
                    ".time {\n"
                        "float: left;\n"
                        "width:10em;\n"
                        "background-color:transparent;\n"
                    "}\n"
                    ".Button {\n"
                    "    display:inline;\n"
                    "    width:10em;\n"
                    "    border:none;\n"
                    "    border-radius: 5px;\n"
                    "    padding:4px;\n"
                    "    margin:2px;\n"
                    "    cursor: hand;\n"
                    "}\n"
            );

            wire::string header( $quote(
                    <html>
                        <head>
                        <script src='http://code.jquery.com/jquery-1.11.0.min.js'></script>
                        <link rel='stylesheet' type='text/css' href='.tint.colors.css' />
                        <script language='javascript'>\1</script>
                        <style type='text/css'>\2</style>
                    </head>
                    <body>
                    <div id='toc'>
                        <a href='http://en.wikipedia.org/wiki/Same_origin_policy'><u>SOP issue found</u></a>
                        <xmp>Chrome user? Try Mozilla or add --allow-file-access-from-files to your Chrome cmdline</xmp>
                    </div>
                    <div id='interface'></div>
                    <div id='pre'></div>
                    <div id='contents'></div>
                    <div id='post'></div>
                    <script>
                        $.support.cors = true;
                        $('#toc').load('.tint.toc.ajax', function() {
                            $('#pre').html( "<b>no log selected</b>" );
                            $('#post').html( "" );
                        });
                        $('#interface').load('.tint.interface.ajax', function() {
                            $('.link').each( function(){
                                $(this).click( function() {
                                    $('#contents').load( $(this).html() );
                                    $('#pre').html( 'Click on buttons to toggle visibility. Click on STACK buttons to toggle visibility of stack traces.<br/><b>' + $(this).text() + '</b>' );
                                });
                             });
                        });
                    </script>
                ),
                jscript,
                css
            );

            sao::file( indexname ).overwrite( header );
            sao::file( ".tint.toc.ajax" ).append( wire::string("<a class='link' href='javascript:;'>\1</a>\n", filename ) );

//            readme();
        }
        else
        if( close )
        {
            sao::file( filename ).append( cache + "\n" );
            cache = std::string();
        }
        else
        if( feed )
        {
            // html log

            int depth_level = 0;

            #if defined(_WIN32) && defined(_MSC_VER)
            depth_level = 0;
            #endif

            // @todo: find a better (faster) way to do this {
            wire::strings tags = cache.lowercase().tokenize(" !|:;,.@#~$%&/(){}[]+-*\\");
            wire::string tagline = tags.str("class_\1 ");
            // }

            if( tags.size() > 0 ) // @todo: sometimes cache string is blank... why? fix it
            {
                static size_t id = 0;

                std::string stack; //( resolve_stack ? moon9::callstack().str("\2\n", depth_level) : std::string() );

                sao::file( filename ).append(
                    wire::string(
                        "<p class=\"\5\"><span class=\"time\">\4</span><a onclick=\"hide('trace\1')\">STACK</a> \2</p>"
                        "<pre id=\"trace\1\">\3</pre>\n", ++id, cache, stack, dt.s(), tagline
                    )
                );

                cache = std::string();
            }
        }
        else
        {
            cache += line;
        }
    }

    std::ostream &io = sao::stream::make(html::custom_logger);
}

