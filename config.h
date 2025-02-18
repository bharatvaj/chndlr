/* See LICENSE file for copyright and license details. */

#define CHNDLR_FALLBACK_CMD "firefox"

static const Pair pairs[] = {
	/* regex                  action */
	/* files */
	{ "\\.(jpg|png|tiff)$",    "nsxiv %s"        },
	{ "\\.gif$",               "wget -O /tmp/tmp_gifview.gif %s && gifview -a /tmp/tmp_gifview.gif" },
	{ "\\.mp3$",               "st -e mplayer %s" },

	/* web */
#define WEB_PREFIX "^(http|https)://?(www\.)?"
	/* youtube */
	{ WEB_PREFIX "(youtube.com/watch\?|youtu\.be/)", "mpv %s" },

	/* github */
	{ WEB_PREFIX "(github.com)/([^/]+)/([^/]+)/actions", "gh run list --repo %1://%2%3/%4/%5" },
	{ WEB_PREFIX "(github.com)", "lynx %s" },
};
