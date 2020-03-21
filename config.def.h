/* See LICENSE file for copyright and license details. */

/* appearance */
static unsigned int borderpx               = 1;        /* border pixel of windows */
static unsigned int snap                   = 32;       /* snap pixel */
static int showbar                         = 1;        /* 0 means no bar */
static int topbar                          = 1;        /* 0 means bottom bar */
static const char *fonts[]                 = { "monospace:size=10" };
static const char dmenufont[]              = "monospace:size=10";
static const char col_gray1[]              = "#222222";
static const char col_gray2[]              = "#444444";
static const char col_gray3[]              = "#bbbbbb";
static const char col_gray4[]              = "#eeeeee";
static const char col_cyan[]               = "#005577";
static const unsigned int normfgalpha      = OPAQUE;
static const unsigned int selfgalpha       = OPAQUE;
static const unsigned int normborderalpha  = OPAQUE;
static const unsigned int selborderalpha   = OPAQUE;
static const unsigned int normbgalpha      = 0xd0;
static const unsigned int selbgalpha       = 0xd0;
static const char *colors[][3]      = {
	/*               fg         bg         border   */
	[SchemeNorm] = { col_gray3, col_gray1, col_gray2 },
	[SchemeSel]  = { col_gray4, col_cyan,  col_cyan  },
};
static unsigned int alphas[][3] = {
	/*               fg      bg        border     */
	[SchemeNorm] = { normfgalpha, normbgalpha, normborderalpha },
	[SchemeSel]  = { selfgalpha,  selbgalpha,  selborderalpha  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class      instance    title       tags mask     isfloating   monitor */
	{ "Gimp",     NULL,       NULL,       0,            1,           -1 },
	{ "Firefox",  NULL,       NULL,       1 << 8,       0,           -1 },
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 1;    /* 1 means respect size hints in tiled resizals */

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ "[M]",      monocle },
};

/* key definitions */
#define MODKEY Mod1Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      { view,           { TAG } } }, \
	{ MODKEY|ControlMask,           KEY,      { toggleview,     { TAG } } }, \
	{ MODKEY|ShiftMask,             KEY,      { tag,            { TAG } } }, \
	{ MODKEY|ControlMask|ShiftMask, KEY,      { toggletag,      { TAG } } },


/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run" };
static const char *termcmd[]  = { "st", NULL };

/* commands */
static const Key keys[] = {
	/* modifier                     key        function        argument */
	{ MODKEY,                       XK_p,      {spawn,          {"dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", col_gray1, "-nf", col_gray3, "-sb", col_cyan, "-sf", col_gray4, NULL}}},
	{ MODKEY|ShiftMask,             XK_Return, {spawn,          {"st", NULL}} },
	{ MODKEY,                       XK_b,      {togglebar,      {NULL}}    },
	{ MODKEY,                       XK_j,      {focusstack,     {"+1"}}    },
	{ MODKEY,                       XK_k,      {focusstack,     {"-1"}}    },
	{ MODKEY,                       XK_i,      {incnmaster,     {"+1"}}    },
	{ MODKEY,                       XK_d,      {incnmaster,     {"-1"}}    },
	{ MODKEY,                       XK_h,      {setmfact,       {"-0.05"}} },
	{ MODKEY,                       XK_l,      {setmfact,       {"+0.05"}} },
	{ MODKEY,                       XK_Return, {zoom,           {NULL}}    },
	{ MODKEY,                       XK_Tab,    {view,           {NULL}}    },
	{ MODKEY|ShiftMask,             XK_c,      {killclient,     {NULL}}    },
	{ MODKEY,                       XK_t,      {setlayout,      {"[]="}}   },
	{ MODKEY,                       XK_f,      {setlayout,      {"><>"}}   },
	{ MODKEY,                       XK_m,      {setlayout,      {"[M]"}}   },
	{ MODKEY,                       XK_space,  {setlayout,      {NULL}}    },
	{ MODKEY|ShiftMask,             XK_space,  {togglefloating, {NULL}}    },
	{ MODKEY,                       XK_0,      {viewall,        {NULL}}    },
	{ MODKEY|ShiftMask,             XK_0,      {tagall,         {NULL}}    },
	{ MODKEY,                       XK_comma,  {focusmon,       {"-1"}}    },
	{ MODKEY,                       XK_period, {focusmon,       {"+1"}}    },
	{ MODKEY|ShiftMask,             XK_comma,  {tagmon,         {"-1"}}    },
	{ MODKEY|ShiftMask,             XK_period, {tagmon,         {"+1"}}    },
	{ MODKEY,                       XK_F5,     {xresources,     {NULL}}    },
	TAGKEYS(                        XK_1,                       "0")
	TAGKEYS(                        XK_2,                       "1")
	TAGKEYS(                        XK_3,                       "2")
	TAGKEYS(                        XK_4,                       "3")
	TAGKEYS(                        XK_5,                       "4")
	TAGKEYS(                        XK_6,                       "5")
	TAGKEYS(                        XK_7,                       "6")
	TAGKEYS(                        XK_8,                       "7")
	TAGKEYS(                        XK_9,                       "8")
	{ MODKEY|ShiftMask,             XK_q,      quit,            {NULL}     },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
const Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        {setlayout,      {NULL}}       },
	{ ClkLtSymbol,          0,              Button3,        {setlayout,      {"[]="}}      },
	{ ClkWinTitle,          0,              Button2,        {zoom,           {NULL}}       },
	{ ClkStatusText,        0,              Button2,        {spawn,          {"st", NULL}} },
	{ ClkClientWin,         MODKEY,         Button1,        {movemouse,      {NULL}}       },
	{ ClkClientWin,         MODKEY,         Button2,        {togglefloating, {NULL}}       },
	{ ClkClientWin,         MODKEY,         Button3,        {resizemouse,    {NULL}}       },
	{ ClkTagBar,            0,              Button1,        {view,           {NULL}}       },
	{ ClkTagBar,            0,              Button3,        {toggleview,     {NULL}}       },
	{ ClkTagBar,            MODKEY,         Button1,        {tag,            {NULL}}       },
	{ ClkTagBar,            MODKEY,         Button3,        {toggletag,      {NULL}}       },
};

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
	{ "font",             STRING,   &fonts[0]              },
	{ "normbgcolor",      STRING,   &colors[SchemeNorm][1] },
	{ "normfgcolor",      STRING,   &colors[SchemeNorm][0] },
	{ "normbordercolor",  STRING,   &colors[SchemeNorm][2] },
	{ "selfgcolor",       STRING,   &colors[SchemeSel][0]  },
	{ "selbgcolor",       STRING,   &colors[SchemeSel][1]  },
	{ "selbordercolor",   STRING,   &colors[SchemeSel][2]  },
	{ "normbgalpha",      STRING,   &alphas[SchemeNorm][1] },
	{ "normfgalpha",      STRING,   &alphas[SchemeNorm][0] },
	{ "normborderalpha",  STRING,   &alphas[SchemeNorm][2] },
	{ "selfgalpha",       STRING,   &alphas[SchemeSel][0]  },
	{ "selbgalpha",       STRING,   &alphas[SchemeSel][1]  },
	{ "selborderalpha",   STRING,   &alphas[SchemeSel][2]  },
	{ "showbar",          INTEGER,  &showbar               },
	{ "topbar",           INTEGER,  &topbar                },
	{ "borderpx",         INTEGER,  &borderpx              },
	{ "snap",             INTEGER,  &snap                  },
};

static Cmd commands[] = {
	/* create [cmd]: create a new window, run `cmd` in the shell if specified */
	/*{ "create", { create,	{ NULL } } },*/
	/* focus <win_id>: focus the window whose `DVTM_WINDOW_ID` is `win_id` */
	{ "focusmon",       { focusmon,       { NULL } } },
	{ "focusstack",     { focusstack,     { NULL } } },
	{ "killclient",     { killclient,     { NULL } } },
	{ "quit",           { quit,           { NULL } } },
	{ "setlayout",      { setlayout,      { NULL } } },
	{ "setmfact",       { setmfact,       { NULL } } },
	{ "spawn",          { spawn,          { NULL } } },
	{ "tag",            { tag,            { NULL } } },
	{ "tagmon",         { tagmon,         { NULL } } },
	{ "togglebar",      { togglebar,      { NULL } } },
	{ "togglefloating", { togglefloating, { NULL } } },
	{ "toggletag",      { toggletag,      { NULL } } },
	{ "toggletagset",   { toggletagset,   { NULL } } },
	{ "toggleview",     { toggleview,     { NULL } } },
	{ "view",           { view,           { NULL } } },
	{ "viewall",        { viewall,        { NULL } } },
	{ "zoom",           { zoom,           { NULL } } },
        { "incnmaster",     { incnmaster,     { NULL } } },
        { "tagall",         { tagall,         { NULL } } },
        { "togglebar",      { togglebar,      { NULL } } },
	/* tag <win_id> <tag> [tag ...]: add +tag, remove -tag or set tag of the window with the given identifier */
	/*{ "tag",    { tagid,	{ NULL } } },*/
};
