/* See LICENSE file for copyright and license details. */

const char* chndlr_fallback_cmd = "firefox ";

#define P(RE,...) { RE, (const char*[]) { __VA_ARGS__, NULL} }

#define WEB_PREFIX(URL) "^(https?://www\\." URL "|https?://" URL ")"

static const Pair pairs[] = {
	/* regex                  action */

	/* files */
	P( "\\.(jpg|png|tiff|gif)$","nsxiv", "%s"        ),
	P( "\\.mp3$",               "st", "-e", "mplayer", "%s"),

	/* web */

	/* youtube */
	P( WEB_PREFIX("youtube.com/watch\\?|youtu\\.be/"), "mpv", "%s"),

	/* github */
	P( WEB_PREFIX("github.com") "/([^/]+)/([^/]+)/actions",
								"gh", "run", "list", "--repo", "%1/%2/%3" ),
	P( WEB_PREFIX("github.com"), "lynx", "%s" ),
};

#undef P
#undef WEB_PREFIX
