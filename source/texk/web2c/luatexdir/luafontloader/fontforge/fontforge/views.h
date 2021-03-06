/* Copyright (C) 2000-2008 by George Williams */
/*
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:

 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.

 * The name of the author may not be used to endorse or promote products
 * derived from this software without specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef _VIEWS_H
#define _VIEWS_H

#include "baseviews.h"

#include <ggadget.h>

struct gfi_data;
struct contextchaindlg;
struct statemachinedlg;

extern struct cvshows {
    int showfore, showback, showgrids, showhhints, showvhints, showdhints;
    int showpoints, showfilled;
    int showrulers;
    int showrounds;		/* 0=>no, 1=>auto, 2=>always */
    int showmdx, showmdy;	/* minimum distances x,y */
    int showhmetrics, showvmetrics;	/* show advance width, baseline, etc. */
    int markextrema;
    int markpoi;		/* Points of inflection */
    int showblues, showfamilyblues;
    int showanchor;
    int showcpinfo;
    int showtabs;		/* with the names of former glyphs */
    int showsidebearings;
    int showrefnames;
    int snapoutlines;
    int showalmosthvlines;
    int showalmosthvcurves;
    int hvoffset;
} CVShows;

extern struct bvshows {
    int showfore, showoutline, showgrid;
    int lastpixelsize;
} BVShows;

enum debug_wins { dw_registers=0x1, dw_stack=0x2, dw_storage=0x4, dw_points=0x8,
	dw_cvt=0x10, dw_raster=0x20, dw_gloss=0x40 };

struct instrinfo {
    int isel_pos;
    int16 lheight,lpos;
    char *scroll, *offset;
    GWindow v;
    GGadget *vsb;
    int16 sbw;
    int16 vheight, vwidth;
    int16 lstopped;
    int16 as, fh;
    struct instrdata *instrdata;
    GFont *gfont;
    unsigned int showaddr: 1;
    unsigned int showhex: 1;
    unsigned int mousedown: 1;
    void *userdata;
    void (*selection_callback)(struct instrinfo *,int ip);
    int  (*bpcheck)(struct instrinfo *,int ip);
    int  (*handle_char)(struct instrinfo *,GEvent *e);
};

typedef struct debugview {
    struct debugger_context *dc;	/* Local to freetype.c */
    GWindow dv, v;
    /* Windows for twilight points, cvt, registers, stack, storage, stack gloss */
    GWindow regs, stack, storage, points, cvt, raster, gloss;	/* order matters */
    GWindow points_v;
    GGadget *cvtsb;
    GGadget *pts_vsb;
    GGadget *glosssb;
    GGadget *storagesb;
    GGadget *regsb;
    GGadget *stacksb;
    struct instrdata id;
    struct instrinfo ii;
    int dwidth, toph;
    struct charview *cv;
    double scalex, scaley;
    int pts_head, cvt_offtop, gloss_offtop, storage_offtop, stack_offtop, reg_offtop;
    int points_offtop;

    int codeSize;
    uint8 initialbytes[4];
    struct reflist { RefChar *ref; struct reflist *parent; } *active_refs;
    int last_npoints;
    int layer;
} DebugView;

enum dv_coderange { cr_none=0, cr_fpgm, cr_prep, cr_glyph };	/* cleverly chosen to match ttobjs.h */

typedef struct charview {
    CharViewBase b;
    uint32 showback[BACK_LAYER_MAX/32];
    unsigned int showfore:1;
    unsigned int showgrids:1;
    unsigned int showhhints:1;
    unsigned int showvhints:1;
    unsigned int showdhints:1;
    unsigned int showpoints:1;
    unsigned int showfilled:1;
    unsigned int showrulers:1;
    unsigned int showrounds:2;		/* 0=>no, 1=>auto, 2=>always */
    unsigned int showmdx:1;
    unsigned int showmdy:1;
    unsigned int showhmetrics:1;
    unsigned int showvmetrics:1;
    unsigned int showblues:1;	/* 16 */
    unsigned int showfamilyblues:1;
    unsigned int showanchor:1;
    unsigned int showpointnumbers:1;
    unsigned int markextrema:1;
    unsigned int markpoi:1;
    unsigned int needsrasterize:1;		/* Rasterization (of fill or fontview) needed on mouse up */
    unsigned int recentchange:1;		/* a change happened in the grids or background. don't need to rasterize */
    unsigned int info_within: 1;		/* cursor is within main window */
    unsigned int back_img_out_of_date: 1;	/* Force redraw of back image pixmap */
    unsigned int cntrldown:1;
    unsigned int joinvalid:1;
    unsigned int widthsel:1;
    unsigned int vwidthsel:1;
    unsigned int icsel:1;
    unsigned int tah_sel:1;
    unsigned int inactive:1;			/* When in a search view */
    unsigned int show_ft_results: 1;	/* 32 */
    unsigned int coderange: 2;			/* For the debugger */
    unsigned int autonomous_ruler_w: 1;
    unsigned int showcpinfo: 1;
    unsigned int showtabs: 1;
    unsigned int showsidebearings: 1;
    unsigned int showing_spiro_pt_menu: 1;
    unsigned int ruler_pressed: 1;
    unsigned int ruler_pressedv: 1;
    unsigned int showrefnames: 1;
    unsigned int snapoutlines: 1;
    unsigned int showalmosthvlines: 1;
    unsigned int showalmosthvcurves: 1;
    int hvoffset;		/* for showalmosthvlines */
    int layers_off_top;
    real scale;
    GWindow gw, v;
    GGadget *vsb, *hsb, *mb, *tabs;
    GFont *small, *normal;
    GWindow icon;
    GWindow ruler_w;
    GFont *rfont;
    GTimer *pressed;
    GWindow backimgs;
    GIC *gic;
    GIC *gwgic;
    int width, height;
    int xoff, yoff;
    int mbh, infoh, rulerh;
    int16 sas, sfh, sdh, nas, nfh;
    BasePoint info;
    SplinePoint *info_sp;
    Spline *info_spline;
    real info_t;
    GPoint e;					/* mouse location */
    GPoint olde;
    BasePoint last_c;
    BDFChar *filled;
    GImage gi;					/* used for fill bitmap only */
    int enc;
    EncMap *map_of_enc;				/* Only use for comparison against fontview's map to see if our enc be valid */
						/*  Will not be updated when fontview is reencoded */
    SplinePoint *lastselpt;
    /*GWindow tools, layers;*/
    int8 b1_tool, cb1_tool, b2_tool, cb2_tool;		/* Button 3 does a popup */
    int8 s1_tool, s2_tool, er_tool;			/* Bindings for wacom stylus and eraser */
    int8 showing_tool, pressed_tool, pressed_display, had_control, active_tool;
    SplinePointList *active_spl;
    SplinePoint *active_sp;
    IPoint handscroll_base;
    uint16 rfh, ras;
    BasePoint lastknife;
    struct freehand {
	struct tracedata *head, *last;	/* for the freehand tool */
	SplinePointList *current_trace;
	int ignore_wobble;		/* Ignore wiggles smaller than this */
	int skip_cnt;
    } freehand;
    enum expandedge { ee_none, ee_nw, ee_up, ee_ne, ee_right, ee_se, ee_down,
	    ee_sw, ee_left, ee_max } expandedge;
    BasePoint expandorigin;
    real expandwidth, expandheight;
    SplinePointList *active_shape;
    SplinePoint joinpos;
    SplineChar *template1, *template2;
#if HANYANG
    struct jamodisplay *jamodisplay;
#endif
    real oldwidth, oldvwidth;
    int16 oldic, oldtah;
#if _ModKeysAutoRepeat
    GTimer *autorpt;
    int keysym, oldstate;
    int oldkeyx, oldkeyy;
    GWindow oldkeyw;
#endif
    PST *lcarets;
    int16 nearcaret;
	/* freetype results display */
    int16 ft_dpi, ft_ppemy, ft_ppemx, ft_depth;
    real ft_pointsizey, ft_pointsizex;
    struct freetype_raster *raster, *oldraster;
    DebugView *dv;
    uint32 mmvisible;
    char *former_names[FORMER_MAX];
    int former_cnt;
    AnchorPoint *apmine, *apmatch;
    SplineChar *apsc;
    int guide_pos;
} CharView;

typedef struct bitmapview {
    BDFChar *bc;
    BDFFont *bdf;
    struct fontview *fv;
    EncMap *map_of_enc;
    int enc;
    GWindow gw, v;
    GGadget *vsb, *hsb, *mb;
    GGadget *recalc;
    GFont *small;
    int xoff, yoff;
    int width, height;
    int infoh, mbh;
    int scale;
    real scscale;
    struct bitmapview *next;
    unsigned int showfore:1;
    unsigned int showoutline:1;
    unsigned int showgrid:1;
    unsigned int cntrldown:1;
    unsigned int recentchange:1;
    unsigned int clearing:1;
    unsigned int shades_hidden:1;
    unsigned int shades_down:1;
    /*GWindow tools, layers;*/
    int8 b1_tool, cb1_tool, b2_tool, cb2_tool;		/* Button 3 does a popup */
    int8 s1_tool, s2_tool, er_tool;			/* Bindings for wacom stylus and eraser */
    int8 showing_tool, pressed_tool, pressed_display, had_control, active_tool;
    int pressed_x, pressed_y;
    int info_x, info_y;
    int event_x, event_y;
    int16 sas, sfh;
#if _ModKeysAutoRepeat
    GTimer *autorpt;
    int keysym, oldstate;
#endif
    int color;			/* for greyscale fonts (between 0,255) */
    int color_under_cursor;
} BitmapView;

struct aplist { AnchorPoint *ap; int connected_to, selected; struct aplist *next; };

enum mv_grids { mv_hidegrid, mv_showgrid, mv_partialgrid, mv_hidemovinggrid };
enum mv_type { mv_kernonly, mv_widthonly, mv_kernwidth };
typedef struct metricsview {
    struct fontview *fv;
    SplineFont *sf;
    int pixelsize;
    BDFFont *bdf;		/* We can also see metric info on a bitmap font */
    BDFFont *show;		/*  Or the rasterized version of the outline font */
    GWindow gw, v;
    GFont *font;
    GGadget *hsb, *vsb, *mb, *text, *script, *features, *subtable_list;
    GGadget *namelab, *widthlab, *lbearinglab, *rbearinglab, *kernlab;
    int16 xstart;
    int16 width, height, dwidth;
    int16 vwidth, vheight;
    int16 mbh,sbh;
    int16 topend;		/* y value of the end of the region containing the text field */
    int16 displayend;		/* y value of the end of the region showing filled characters */
    int16 fh, as;
    int16 cmax, clen; 
    SplineChar **chars;		/* Character input stream */
    struct opentype_str *glyphs;/* after going through the various gsub/gpos transformations */
    struct metricchar {		/* One for each glyph above */
	int16 dx, dwidth;	/* position and width of the displayed char */
	int16 dy, dheight;	/*  displayed info for vertical metrics */
	int xoff, yoff;
	int16 mx, mwidth;	/* position and width of the text underneath */
	int16 kernafter;
	unsigned int selected: 1;
	GGadget *width, *lbearing, *rbearing, *kern, *name;
    } *perchar;
    SplineChar **sstr;		/* Character input stream */
    int16 mwidth, mbase;
    int16 glyphcnt, max;
    int16 pressed_x, pressed_y;
    int16 activeoff;
    int xoff, coff, yoff;
    struct metricsview *next;
    unsigned int right_to_left: 1;
    unsigned int pressed: 1;
    unsigned int pressedwidth: 1;
    unsigned int pressedkern: 1;
    unsigned int showgrid: 2;
    unsigned int antialias: 1;
    unsigned int vertical: 1;
    unsigned int type: 2;		/* enum mv_type */
    int xp, yp, ap_owner;
    BasePoint ap_start;
    int cursor;
    int scale_index;
    struct lookup_subtable *cur_subtable;
    GTextInfo *scriptlangs;
    int word_index;
    int layer;
    int fake_unicode_base;
    GIC *gwgic;
} MetricsView;

enum fv_metrics { fvm_baseline=1, fvm_origin=2, fvm_advanceat=4, fvm_advanceto=8 };
typedef struct fontview {
    FontViewBase b;
    BDFFont *show, *filled;
    GWindow gw, v;
    GFont **fontset;
    GGadget *vsb, *mb;
    GTimer *pressed;
    GTimer *resize;
    GEvent resize_event;
    GIC *gic;
    GIC *gwgic;
    int width, height;		/* of v */
    int16 infoh,mbh;
    int16 lab_height, lab_as;
    int16 colcnt, rowcnt;		/* of display window */
    int32 rowoff, rowltot;		/* Can be really big in full unicode */
    int16 cbw,cbh;			/* width/height of a character box */
    int pressed_pos, end_pos;
    unsigned int antialias:1;
    unsigned int bbsized:1;		/* displayed bitmap should be scaled by bounding box rather than emsize */
    unsigned int wasonlybitmaps:1;
    /*unsigned int refstate: 3;*/	/* 0x1 => paste orig of all non exist refs, 0x2=>don't, 0x3 => don't warn about non-exist refs with no source font */
    unsigned int touched: 1;
    unsigned int showhmetrics: 4;
    unsigned int showvmetrics: 4;
    unsigned int drag_and_drop: 1;
    unsigned int has_dd_no_cursor: 1;
    unsigned int any_dd_events_sent: 1;
    unsigned int resize_expected: 1;
	/* Some window managers do not honour my resize requests (if window is*/
	/*  maximized for example), but we depend on the resize request to    */
	/*  fix up the window. We do get a configure notify, but the window   */
	/*  stays the same size, so kludge things */
    unsigned int glyphlabel: 2;
    int16 magnify;
    int16 user_requested_magnify;
    struct searchview *sv;
    SplineChar *sc_near_top;
    int sel_index;
    struct lookup_subtable *cur_subtable;
} FontView;

typedef struct findsel {
    GEvent *e;
    real fudge;		/* One pixel fudge factor */
    real xl,xh, yl, yh;	/* One pixel fudge factor */
    real c_xl,c_xh, c_yl, c_yh;		/* fudge rectangle for control points, larger than above if alt is depressed */
    unsigned int select_controls: 1;	/* notice control points */
    unsigned int seek_controls: 1;	/* notice control points before base points */
    unsigned int all_controls: 1;	/* notice control points even if the base points aren't selected (in truetype point numbering mode where all cps are visible) */
    real scale;
} FindSel;

typedef struct searchview {
    struct cvcontainer base;
    FontView dummy_fv;
    SplineFont dummy_sf;
    LayerInfo layerinfo[2];
    SplineChar *chars[2];
    EncMap dummy_map;
    int32 map[2], backmap[2];
    uint8 sel[2];
    CharView cv_srch, cv_rpl;
    CharView *lastcv;
/* ****** */
    GWindow gw;
    GGadget *mb;
    GFont *plain, *bold;
    int mbh;
    int fh, as;
    int rpl_x, cv_y;
    int cv_width, cv_height;
    short button_height, button_width;
/* ****** */
    SearchData sd;
    unsigned int showsfindnext: 1;
    unsigned int findenabled: 1;
    unsigned int rplallenabled: 1;
    unsigned int rplenabled: 1;
    unsigned int isvisible: 1;
} SearchView;

typedef struct mathkernview {
    struct cvcontainer base;
    FontView dummy_fv;
    SplineFont dummy_sf;
    LayerInfo layerinfo[2];
    SplineChar sc_topright, sc_topleft, sc_bottomright, sc_bottomleft;
    SplineChar *chars[4];
    EncMap dummy_map;
    int32 map[4], backmap[4];
    uint8 sel[4];
    CharView cv_topright, cv_topleft, cv_bottomright, cv_bottomleft;
    CharView *lastcv;
/* ****** */
    GWindow gw;
    GWindow cvparent_w;
    GGadget *mb;
    GFont *plain, *bold;
    int mbh;
    int fh, as;
    int mid_space, cv_y;
    int cv_width, cv_height;
    short button_height, button_width;
/* ****** */
    SplineChar *cursc;
    int def_layer;
    struct mathkern *orig_mathkern;
    uint8 saved_mathkern;		/* Can't just check if orig is non-NULL, because NULL is a perfectly valid initial state */
    uint8 last_aspect;
    uint8 done;
} MathKernDlg;

# ifdef FONTFORGE_CONFIG_TILEPATH

typedef struct tilepathdlg {
    struct cvcontainer base;
    FontView dummy_fv;
    SplineFont dummy_sf;
    LayerInfo layerinfo[2];
    SplineChar sc_first, sc_medial, sc_final, sc_isolated;
    SplineChar *chars[4];
    EncMap dummy_map;
    int32 map[4], backmap[4];
    uint8 sel[4];
    CharView cv_first, cv_medial, cv_final, cv_isolated;
    CharView *lastcv;
/* ****** */
    GWindow gw;
    GGadget *mb;
    GFont *plain, *bold;
    int mbh;
    int fh, as;
    int mid_space, cv_y;
    int cv_width, cv_height;
/* ****** */
    struct tiledata *td;
    SplineFont *base_sf;
    uint8 done, oked;
} TilePathDlg;
extern void TPDChar(TilePathDlg *tpd, GEvent *event);
extern void TPDCharViewInits(TilePathDlg *tpd, int cid);
extern void PTDChar(TilePathDlg *tpd, GEvent *event);
extern void PTDCharViewInits(TilePathDlg *tpd, int cid);
#endif		/* Tile Path */

# ifdef FONTFORGE_CONFIG_TYPE3

typedef struct gradientdlg {
    struct cvcontainer base;
    FontView dummy_fv;
    SplineFont dummy_sf;
    LayerInfo layerinfo[2];
    SplineChar sc_grad;
    SplineChar *chars[1];
    EncMap dummy_map;
    int32 map[1], backmap[1];
    uint8 sel[1];
    CharView cv_grad;
/* ****** */
    GWindow gw;
    GGadget *mb;
    GFont *plain, *bold;
    int mbh;
    int fh, as;
    int mid_space, cv_y;
    int cv_width, cv_height;
/* ****** */
    uint8 done, oked;
    struct gradient *active;
} GradientDlg;
extern void GDDChar(GradientDlg *tpd, GEvent *event);
extern void GDDCharViewInits(GradientDlg *gdd,int cid);
#endif		/* Tile Path */

struct lkdata {
    int cnt, max;
    int off_top, off_left;
    struct lkinfo {
	OTLookup *lookup;
	unsigned int open: 1;
	unsigned int deleted: 1;
	unsigned int new: 1;
	unsigned int selected: 1;
	unsigned int moved: 1;
	int16 subtable_cnt, subtable_max;
	struct lksubinfo {
	    struct lookup_subtable *subtable;
	    unsigned int deleted: 1;
	    unsigned int new: 1;
	    unsigned int selected: 1;
	    unsigned int moved: 1;
	} *subtables;
    } *all;
};

struct gfi_data {		/* FontInfo */
    SplineFont *sf;
    int def_layer;
    GWindow gw;
    int tn_active;
    int private_aspect, ttfv_aspect, tn_aspect, tx_aspect, unicode_aspect;
    int old_sel, old_aspect, old_lang, old_strid;
    int ttf_set, names_set, tex_set;
    struct psdict *private;
    int langlocalecode;	/* MS code for the current locale */
    unsigned int family_untitled: 1;
    unsigned int human_untitled: 1;
    unsigned int done: 1;
    unsigned int mpdone: 1;
    unsigned int lk_drag_and_drop: 1;
    unsigned int lk_dropablecursor: 1;
    struct anchor_shows { CharView *cv; SplineChar *sc; int restart; } anchor_shows[2];
    struct texdata texdata;
    struct contextchaindlg *ccd;
    struct statemachinedlg *smd;
/* For GDEF Mark Attachment Class -- used in lookup flags */
/* As usual, class 0 is unused */
    int mark_class_cnt;
    char **mark_classes;		/* glyph name list */
    char **mark_class_names;		/* used within ff */
    struct markclassdlg *mcd;
    GFont *font;
    int as, fh;
    struct lkdata tables[2];
    int lkwidth, lkheight;
    int first_sel_lookup, first_sel_subtable;
};


extern void FVMarkHintsOutOfDate(SplineChar *sc);
extern void FVRefreshChar(FontView *fv,int gid);
extern int _FVMenuSave(FontView *fv);
extern int _FVMenuSaveAs(FontView *fv);
extern int _FVMenuGenerate(FontView *fv,int family);
extern void _FVCloseWindows(FontView *fv);
extern char *GetPostscriptFontName(char *defdir,int mult);
extern void MergeKernInfo(SplineFont *sf,EncMap *map);
#ifdef FONTFORGE_CONFIG_WRITE_PFM
extern int WritePfmFile(char *filename,SplineFont *sf, int type0, EncMap *map);
#endif
extern int SFGenerateFont(SplineFont *sf,int layer, int family,EncMap *map);

extern void NonLinearDlg(FontView *fv,struct charview *cv);
extern void FVChangeChar(FontView *fv,int encoding);
extern void FVMergeFonts(FontView *fv);
extern void FVInterpolateFonts(FontView *fv);

extern void FVDeselectAll(FontView *fv);

extern void FVAutoKern(FontView *fv);
extern void FVAutoWidth(FontView *fv);

extern void SC_MarkInstrDlgAsChanged(SplineChar *sc);

extern void PythonUI_Init(void);

extern void SCStroke(SplineChar *sc);

extern void PfaEditSetFallback(void);
extern void RecentFilesRemember(char *filename);


struct debugger_context;
extern void DebuggerTerminate(struct debugger_context *dc);
extern void DebuggerReset(struct debugger_context *dc,real pointsizey, real pointsizex,int dpi,int dbg_fpgm, int is_bitmap);
extern struct debugger_context *DebuggerCreate(SplineChar *sc,int layer,real pointsizey,real pointsizex,int dpi,int dbg_fpgm, int is_bitmap);
enum debug_gotype { dgt_continue, dgt_step, dgt_next, dgt_stepout };
extern void DebuggerGo(struct debugger_context *dc,enum debug_gotype,DebugView *dv);
extern struct  TT_ExecContextRec_ *DebuggerGetEContext(struct debugger_context *dc);
extern void DebuggerToggleBp(struct debugger_context *dc,int range,int ip);
extern int DebuggerBpCheck(struct debugger_context *dc,int range,int ip);
extern void DebuggerSetWatches(struct debugger_context *dc,int n, uint8 *w);
extern uint8 *DebuggerGetWatches(struct debugger_context *dc, int *n);
extern void DebuggerSetWatchStores(struct debugger_context *dc,int n, uint8 *w);
extern uint8 *DebuggerGetWatchStores(struct debugger_context *dc, int *n);
extern int DebuggerIsStorageSet(struct debugger_context *dc, int index);
extern void DebuggerSetWatchCvts(struct debugger_context *dc,int n, uint8 *w);
extern uint8 *DebuggerGetWatchCvts(struct debugger_context *dc, int *n);
extern int DebuggingFpgm(struct debugger_context *dc);

extern void PrintWindowClose(void);
extern void InsertTextDlg(CharView *cv);

extern char *Kern2Text(SplineChar *other,KernPair *kp,int isv);
extern char *PST2Text(PST *pst,SplineFont *sf);



void EmboldenDlg(FontView *fv, CharView *cv);
void CondenseExtendDlg(FontView *fv, CharView *cv);
void AddSmallCapsDlg(FontView *fv);
void AddSubSupDlg(FontView *fv);
void ObliqueDlg(FontView *fv, CharView *cv);
void GlyphChangeDlg(FontView *fv, CharView *cv, enum glyphchange_type gc);
void ItalicDlg(FontView *fv, CharView *cv);
void ChangeXHeightDlg(FontView *fv,CharView *cv);

extern int FVParseSelectByPST(FontView *fv,struct lookup_subtable *sub,
	int search_type);
extern void DropChars2Text(GWindow gw, GGadget *glyphs,GEvent *event);


extern void FVReplaceOutlineWithReference( FontView *fv, double fudge );
extern void SVDestroy(struct searchview *sv);



extern int  SLICount(SplineFont *sf);
extern unichar_t *ClassName(const char *name,uint32 feature_tag,
	uint16 flags, int script_lang_index, int merge_with, int act_type,
	int macfeature,SplineFont *sf);
extern unichar_t *DecomposeClassName(const unichar_t *clsnm, unichar_t **name,
	uint32 *feature_tag, int *macfeature,
	uint16 *flags, uint16 *script_lang_index,int *merge_with,int *act_type,
	SplineFont *sf);
extern PST *AddSubs(PST *last,uint32 tag,char *name,uint16 flags,
	uint16 sli,SplineChar *sc);


extern void FVScrollToChar(FontView *fv,int i);
extern void FVRegenChar(FontView *fv,SplineChar *sc);
extern FontView *FontNew(void);
extern void _MenuWarnings(GWindow gw,struct gmenuitem *mi,GEvent *e);
extern void MenuPrefs(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuSaveAll(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuExit(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuOpen(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuHelp(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuIndex(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuAbout(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuLicense(GWindow base,struct gmenuitem *mi,GEvent *e);
extern void MenuNew(GWindow gw,struct gmenuitem *mi,GEvent *e);
extern void WindowMenuBuild(GWindow base,struct gmenuitem *mi,GEvent *);
extern void MenuRecentBuild(GWindow base,struct gmenuitem *mi,GEvent *);
extern void MenuScriptsBuild(GWindow base,struct gmenuitem *mi,GEvent *);
extern void mb2DoGetText(GMenuItem2 *mb);
extern void mbDoGetText(GMenuItem *mb);
extern void OFLibBrowse(void);
extern int RecentFilesAny(void);
extern void _aplistbuild(struct gmenuitem *mi,SplineFont *sf,
	void (*func)(GWindow,struct gmenuitem *,GEvent *));
extern int32 *ParseBitmapSizes(GGadget *g,char *msg,int *err);
extern GTextInfo *AddMacFeatures(GTextInfo *opentype,enum possub_type type,SplineFont *sf);
extern unichar_t *AskNameTag(char *title,unichar_t *def,uint32 def_tag,uint16 flags,
	int script_lang_index, enum possub_type type, SplineFont *sf, SplineChar *default_script,
	int merge_with,int act_type);
extern unichar_t *ShowScripts(unichar_t *usedef);
extern GTextInfo *SFLangList(SplineFont *sf,int addfinal,SplineChar *default_script);
extern GTextInfo **SFLangArray(SplineFont *sf,int addfinal);
extern int  ScriptLangList(SplineFont *sf,GGadget *list,int sli);
extern void GListDelSelected(GGadget *list);
extern void GListMoveSelected(GGadget *list,int offset);
extern GTextInfo *GListChangeLine(GGadget *list,int pos, const unichar_t *line);
extern GTextInfo *GListAppendLine(GGadget *list,const unichar_t *line,int select);
extern GTextInfo *GListChangeLine8(GGadget *list,int pos, const char *line);
extern GTextInfo *GListAppendLine8(GGadget *list,const char *line,int select);
extern void CharInfoInit(void);
extern void SCLigCaretCheck(SplineChar *sc,int clean);
extern char *DevTab_Dlg(GGadget *g, int r, int c);
extern int DeviceTableOK(char *dvstr, int *_low, int *_high);
extern void VRDevTabParse(struct vr *vr,struct matrix_data *md);
#ifdef FONTFORGE_CONFIG_DEVICETABLES
extern DeviceTable *DeviceTableParse(DeviceTable *dv,char *dvstr);
extern void DevTabToString(char **str,DeviceTable *adjust);
extern void ValDevTabToStrings(struct matrix_data *mds,int first_offset,ValDevTab *adjust);
#endif
extern void KpMDParse(SplineFont *sf,SplineChar *sc,struct lookup_subtable *sub,
	struct matrix_data *possub,int rows,int cols,int i);
extern void GFI_LookupEnableButtons(struct gfi_data *gfi, int isgpos);
extern void GFI_LookupScrollbars(struct gfi_data *gfi, int isgpos, int refresh);
extern void FontInfo(SplineFont *sf,int layer,int aspect,int sync);
extern void FontInfoDestroy(SplineFont *sf);
extern void FontMenuFontInfo(void *fv);
extern void GFI_CCDEnd(struct gfi_data *d);
extern struct enc *MakeEncoding(SplineFont *sf, EncMap *map);
extern void LoadEncodingFile(void);
extern void RemoveEncoding(void);
extern void SFPrivateInfo(SplineFont *sf);
extern void FVDelay(FontView *fv,void (*func)(FontView *));
extern void GFI_FinishContextNew(struct gfi_data *d,FPST *fpst, int success);
extern void SCPreparePopup(GWindow gw,SplineChar *sc, struct remap *remap, int enc, int actualuni);
extern void CVDrawSplineSet(CharView *cv, GWindow pixmap, SplinePointList *set,
	Color fg, int dopoints, DRect *clip );
extern GWindow CVMakeTools(CharView *cv);
extern GWindow CVMakeLayers(CharView *cv);
extern GWindow BVMakeTools(BitmapView *bv);
extern GWindow BVMakeLayers(BitmapView *bv);
extern void CVSetLayer(CharView *cv,int layer);
extern int CVPaletteMnemonicCheck(GEvent *event);
extern int TrueCharState(GEvent *event);
extern void CVToolsPopup(CharView *cv, GEvent *event);
extern void BVToolsPopup(BitmapView *bv, GEvent *event);
extern real CVRoundRectRadius(void);
extern int CVRectElipseCenter(void);
extern void CVRectEllipsePosDlg(CharView *cv);
extern real CVStarRatio(void);
extern int CVPolyStarPoints(void);
extern StrokeInfo *CVFreeHandInfo(void);
extern void BVToolsSetCursor(BitmapView *bv, int state,char *device);
extern void CVToolsSetCursor(CharView *cv, int state,char *device);
extern int CVPaletteIsVisible(CharView *cv,int which);
extern void CVPaletteSetVisible(CharView *cv,int which,int visible);
extern void CVPalettesRaise(CharView *cv);
extern void CVLayersSet(CharView *cv);
extern void _CVPaletteActivate(CharView *cv,int force);
extern void CVPaletteActivate(CharView *cv);
extern void CV_LayerPaletteCheck(SplineFont *sf);
extern void CVPalettesHideIfMine(CharView *cv);
extern int BVPaletteIsVisible(BitmapView *bv,int which);
extern void BVPaletteSetVisible(BitmapView *bv,int which,int visible);
extern void BVPaletteActivate(BitmapView *bv);
extern void BVPalettesHideIfMine(BitmapView *bv);
extern void BVPaletteColorChange(BitmapView *bv);
extern void BVPaletteColorUnderChange(BitmapView *bv,int color);
extern void BVPaletteChangedChar(BitmapView *bv);
extern void CVPaletteDeactivate(void);
extern void PalettesChangeDocking(void);
extern int CVPalettesWidth(void);
extern int BVPalettesWidth(void);

extern void CVDoTransform(CharView *cv, enum cvtools cvt );
extern void CVTransFunc(CharView *cv,real transform[6],enum fvtrans_flags);
extern void TransformDlgCreate(void *data,void (*transfunc)(void *,real *,int,BVTFunc *,enum fvtrans_flags),
	int (*getorigin)(void *,BasePoint *,int), int enableback,
	enum cvtools cvt);
extern void BitmapDlg(FontView *fv,SplineChar *sc, int isavail);
extern int SimplifyDlg(SplineFont *sf,struct simplifyinfo *smpl);
extern void CVReviewHints(CharView *cv);
extern void CVCreateHint(CharView *cv,int ishstem,int preserveundoes);
extern void SCRemoveSelectedMinimumDistances(SplineChar *sc,int inx);
extern int CVExport(CharView *cv);
extern int BVExport(BitmapView *bv);

extern void DrawAnchorPoint(GWindow pixmap,int x, int y,int selected);
extern void DefaultY(GRect *pos);
extern void CVDrawRubberRect(GWindow pixmap, CharView *cv);
extern void CVInfoDraw(CharView *cv, GWindow pixmap );
extern void CVChar(CharView *cv, GEvent *event );
extern void PI_ShowHints(SplineChar *sc, GGadget *list, int set);
extern GTextInfo *SCHintList(SplineChar *sc,HintMask *);
extern void CVResize(CharView *cv );
extern CharView *CharViewCreate(SplineChar *sc,FontView *fv,int enc);
extern void CharViewFree(CharView *cv);
extern int CVValid(SplineFont *sf, SplineChar *sc, CharView *cv);
extern void CVSetCharChanged(CharView *cv,int changed);
extern int CVAnySel(CharView *cv, int *anyp, int *anyr, int *anyi, int *anya);
extern int CVAnySelPoints(CharView *cv);
extern void CVSelectPointAt(CharView *cv);
extern int CVClearSel(CharView *cv);
extern int CVSetSel(CharView *cv,int mask);
extern void CVInvertSel(CharView *cv);
extern int CVAllSelected(CharView *cv);
extern SplinePointList *CVAnySelPointList(CharView *cv);
extern int CVOneContourSel(CharView *cv, SplinePointList **_spl,
	RefChar **ref, ImageList **img);
extern void CVImport(CharView *cv);
extern void BVImport(BitmapView *bv);
extern void FVImport(FontView *bv);
extern void CVFindCenter(CharView *cv, BasePoint *bp, int nosel);
extern void CVStroke(CharView *cv);
extern void FVStroke(FontView *fv);
extern void FreeHandStrokeDlg(StrokeInfo *si);
extern void OutlineDlg(FontView *fv, CharView *cv,MetricsView *mv,int isinline);
extern void ShadowDlg(FontView *fv, CharView *cv,MetricsView *mv,int wireframe);
extern void CVTile(CharView *cv);
extern void FVTile(FontView *fv);
extern void CVPatternTile(CharView *cv);
extern void FVPatternTile(FontView *fv);
extern void SCCharInfo(SplineChar *sc,int deflayer,EncMap *map,int enc);
extern void CharInfoDestroy(struct charinfo *ci);
extern SplineChar *SuffixCheck(SplineChar *sc,char *suffix);
extern void SCSubtableDefaultSubsCheck(SplineChar *sc, struct lookup_subtable *sub, struct matrix_data *possub, int col_cnt, int r);
extern GImage *PST_GetImage(GGadget *pstk,SplineFont *sf,int def_layer,
	struct lookup_subtable *sub,int popup_r, SplineChar *sc );
extern GImage *NameList_GetImage(SplineFont *sf,SplineChar *sc,int def_layer,
	char *namelist, int isliga );
extern GImage *GV_GetConstructedImage(SplineChar *sc,int def_layer, struct glyphvariants *gv,
	int is_horiz);
extern GImage *SC_GetLinedImage(SplineChar *sc, int def_layer, int pos, int is_italic_cor);
extern struct glyphvariants *GV_ParseConstruction(struct glyphvariants *gv,
	struct matrix_data *stuff, int rows, int cols);
extern void GV_ToMD(GGadget *g, struct glyphvariants *gv);
extern void CVGetInfo(CharView *cv);
extern void CVPGetInfo(CharView *cv);
extern int  SCUsedBySubs(SplineChar *sc);
extern void SCSubBy(SplineChar *sc);
extern void SCRefBy(SplineChar *sc);
extern void ApGetInfo(CharView *cv, AnchorPoint *ap);
extern void CVMakeClipPath(CharView *cv);
extern void CVAddAnchor(CharView *cv);
extern AnchorClass *AnchorClassUnused(SplineChar *sc,int *waslig);
extern void FVSetWidth(FontView *fv,enum widthtype wtype);
extern void CVSetWidth(CharView *cv,enum widthtype wtype);
extern void CVChangeSC(CharView *cv, SplineChar *sc );
extern Undoes *CVPreserveTState(CharView *cv);
extern void CVRestoreTOriginalState(CharView *cv);
extern void CVUndoCleanup(CharView *cv);

extern void CVAdjustPoint(CharView *cv, SplinePoint *sp);
extern void CVMergeSplineSets(CharView *cv, SplinePoint *active, SplineSet *activess,
	SplinePoint *merge, SplineSet *mergess);
extern void CVAdjustControl(CharView *cv,BasePoint *cp, BasePoint *to);
extern int  CVMoveSelection(CharView *cv, real dx, real dy, uint32 input_state);
extern int  CVTestSelectFromEvent(CharView *cv,GEvent *event);
extern void CVMouseUpPoint(CharView *cv,GEvent *event);
extern int  CVMouseMovePointer(CharView *cv, GEvent *event);
extern void CVMouseDownPointer(CharView *cv, FindSel *fs, GEvent *event);
extern void CVMouseDownRuler(CharView *cv, GEvent *event);
extern void CVMouseMoveRuler(CharView *cv, GEvent *event);
extern int CVMouseAtSpline(CharView *cv,GEvent *event);
extern void CVMouseUpRuler(CharView *cv, GEvent *event);
extern void CVMouseMoveHand(CharView *cv, GEvent *event);
extern void CVMouseDownFreeHand(CharView *cv, GEvent *event);
extern void CVMouseMoveFreeHand(CharView *cv, GEvent *event);
extern void CVMouseUpFreeHand(CharView *cv, GEvent *event);
extern void CVMouseDownShape(CharView *cv,GEvent *event);
extern void CPStartInfo(CharView *cv, GEvent *event);
extern void CPUpdateInfo(CharView *cv, GEvent *event);
extern void CPEndInfo(CharView *cv);
extern void BVChar(BitmapView *cv, GEvent *event );
extern void CVMouseDownPoint(CharView *cv,GEvent *event);
extern void CVMouseUpPointer(CharView *cv );
extern void CVCheckResizeCursors(CharView *cv);
extern void CVMouseDownHand(CharView *cv);
extern void CVMouseUpHand(CharView *cv);
extern void CVMouseDownTransform(CharView *cv);
extern void CVMouseMoveTransform(CharView *cv);
extern void CVMouseUpTransform(CharView *cv);
extern void CVMouseDownKnife(CharView *cv);
extern void CVMouseUpKnife(CharView *cv,GEvent *event);
extern void CVMouseMoveShape(CharView *cv);
extern void CVMouseUpShape(CharView *cv);
extern void LogoExpose(GWindow pixmap,GEvent *event, GRect *r,enum drawmode dm);
extern void CVDebugPointPopup(CharView *cv);

extern int GotoChar(SplineFont *sf,EncMap *map);

extern void CVShowPoint(CharView *cv, BasePoint *me);

extern BitmapView *BitmapViewCreate(BDFChar *bc, BDFFont *bdf, FontView *fv,int enc);
extern BitmapView *BitmapViewCreatePick(int enc, FontView *fv);
extern void BitmapViewFree(BitmapView *bv);
extern void BVMenuRotateInvoked(GWindow gw,struct gmenuitem *mi, GEvent *e);
extern void BVRotateBitmap(BitmapView *bv,enum bvtools type );
extern int  BVColor(BitmapView *bv);
extern void BCGeneralFunction(BitmapView *bv,
	void (*SetPoint)(BitmapView *,int x, int y, void *data),void *data);
extern char *BVFlipNames[];
extern void BVChangeBC(BitmapView *bv, BDFChar *bc, int fitit );

extern void MVSetSCs(MetricsView *mv, SplineChar **scs);
extern void MVRefreshChar(MetricsView *mv, SplineChar *sc);
extern void MVRegenChar(MetricsView *mv, SplineChar *sc);
extern void MVReKern(MetricsView *mv);
extern MetricsView *MetricsViewCreate(FontView *fv,SplineChar *sc,BDFFont *bdf);
extern void MetricsViewFree(MetricsView *mv);
extern void MVRefreshAll(MetricsView *mv);
extern void MV_FriendlyFeatures(GGadget *g, int pos);
extern GTextInfo *SLOfFont(SplineFont *sf);

extern void DoPrefs(void);
extern void GListAddStr(GGadget *list,unichar_t *str, void *ud);
extern void GListReplaceStr(GGadget *list,int index, unichar_t *str, void *ud);
extern struct macname *NameGadgetsGetNames( GWindow gw );
extern void NameGadgetsSetEnabled( GWindow gw, int enable );
extern int GCDBuildNames(GGadgetCreateData *gcd,GTextInfo *label,int pos,struct macname *names);
extern void GCDFillMacFeat(GGadgetCreateData *mfgcd,GTextInfo *mflabels, int width,
	MacFeat *all, int fromprefs, GGadgetCreateData *boxes,
	GGadgetCreateData **array);
extern void Prefs_ReplaceMacFeatures(GGadget *list);

extern unichar_t *FVOpenFont(char *title, const char *defaultfile, int mult);




extern void ShowAboutScreen(void);
extern void DelayEvent(void (*func)(void *), void *data);

extern void FindProblems(FontView *fv,CharView *cv,SplineChar *sc);
extern void CVConstrainSelection(CharView *cv,int type);
extern void CVMakeParallel(CharView *cv);

extern void ScriptDlg(FontView *fv,CharView *cv);

# if HANYANG
extern void MenuNewComposition(GWindow gw, struct gmenuitem *, GEvent *);
extern void CVDisplayCompositions(GWindow gw, struct gmenuitem *, GEvent *);
extern void Disp_DoFinish(struct jamodisplay *d, int cancel);
extern void Disp_RefreshChar(SplineFont *sf,SplineChar *sc);
extern void Disp_DefaultTemplate(CharView *cv);
# endif

extern SearchView *SVCreate(FontView *fv);
extern void SVCharViewInits(SearchView *sv);
extern void SV_DoClose(struct cvcontainer *cvc);
extern void SVChar(SearchView *sv, GEvent *event);
extern void SVMakeActive(SearchView *sv,CharView *cv);
extern int SVAttachFV(FontView *fv,int ask_if_difficult);
extern void SVDetachFV(FontView *fv);

extern void MKDMakeActive(MathKernDlg *mkd,CharView *cv);
extern void MKDChar(MathKernDlg *mkd, GEvent *event);
extern void MKD_DoClose(struct cvcontainer *cvc);
extern void MKDCharViewInits(MathKernDlg *mkd);
extern void MathKernDialog(SplineChar *sc,int def_layer);

extern void ShowAtt(SplineFont *sf,int def_layer);
extern void FontCompareDlg(FontView *fv);
extern void SFShowKernPairs(SplineFont *sf,SplineChar *sc,AnchorClass *ac,int layer);
extern void SFShowLigatures(SplineFont *sf,SplineChar *sc);

extern void SCEditInstructions(SplineChar *sc);
extern void SFEditTable(SplineFont *sf, uint32 tag);
extern void IIScrollTo(struct instrinfo *ii,int ip,int mark_stop);
extern void IIReinit(struct instrinfo *ii,int ip);
extern int ii_v_e_h(GWindow gw, GEvent *event);
extern void instr_scroll(struct instrinfo *ii,struct sbevent *sb);

extern void CVGridFitChar(CharView *cv);
extern void CVFtPpemDlg(CharView *cv,int debug);
extern void SCDeGridFit(SplineChar *sc);
extern void SCReGridFit(SplineChar *sc,int layer);

extern void CVDebugReInit(CharView *cv,int restart_debug,int dbg_fpgm);
extern void CVDebugFree(DebugView *dv);
extern int DVChar(DebugView *dv, GEvent *e);

extern void KernClassD(KernClass *kc, SplineFont *sf, int layer, int isv);
extern void ShowKernClasses(SplineFont *sf,MetricsView *mv,int layer,int isv);
extern void KCLD_End(struct kernclasslistdlg *kcld);
extern void KCLD_MvDetach(struct kernclasslistdlg *kcld,MetricsView *mv);
extern void KernPairD(SplineFont *sf,SplineChar *sc1,SplineChar *sc2,int layer, int isv);
extern void KCD_DrawGlyph(GWindow pixmap,int x,int baseline,BDFChar *bdfc,int mag);
extern GTextInfo *BuildFontList(FontView *except);
extern void TFFree(GTextInfo *tf);

extern void AnchorControl(SplineChar *sc,AnchorPoint *ap,int layer);
extern void AnchorControlClass(SplineFont *_sf,AnchorClass *ac,int layer);

extern void FVSelectByPST(FontView *fv);

enum hist_type { hist_hstem, hist_vstem, hist_blues };
struct psdict;
extern void SFHistogram(SplineFont *sf,int layer, struct psdict *private,uint8 *selected,
	EncMap *map, enum hist_type which);

extern void CCD_Close(struct contextchaindlg *ccd);
extern int CCD_NameListCheck(SplineFont *sf,const char *ret,int empty_bad,char *title);
extern struct contextchaindlg *ContextChainEdit(SplineFont *sf,FPST *fpst,
	struct gfi_data *gfi,unichar_t *newname);
extern int CCD_InvalidClassList(char *ret,GGadget *list,int wasedit);
extern char *cu_copybetween(const unichar_t *start, const unichar_t *end);

extern struct statemachinedlg *StateMachineEdit(SplineFont *sf,ASM *sm,struct gfi_data *d);
extern void SMD_Close(struct statemachinedlg *smd);
extern void GFI_FinishSMNew(struct gfi_data *d,ASM *sm, int success,int isnew);
extern void GFI_SMDEnd(struct gfi_data *d);

extern void MMChangeBlend(MMSet *mm,FontView *fv,int tonew);
extern void MMWizard(MMSet *mm);

extern int LayerDialog(Layer *layer,SplineFont *sf);
extern void CVLayerChange(CharView *cv);

extern int PointOfViewDlg(struct pov_data *pov,SplineFont *sf,int flags);

extern SplineChar *FVMakeChar(FontView *fv,int i);

extern void CVPointOfView(CharView *cv,struct pov_data *);

extern void DVCreateGloss(DebugView *dv);
extern int CVXPos(DebugView *dv,int offset,int width);

extern GMenuItem *GetEncodingMenu(void (*func)(GWindow,GMenuItem *,GEvent *),
	Encoding *current);

extern GTextInfo *TIFromName(const char *name);

enum subtable_data_flags {
    /* I have flags for each alternative because I want "unspecified" to be */
    /*  an option */
    sdf_kernclass      = 0x01,
    sdf_kernpair       = 0x02,
    sdf_verticalkern   = 0x04,
    sdf_horizontalkern = 0x08,
    sdf_dontedit       = 0x10
};
struct subtable_data {
    int flags;
    SplineChar *sc;
};

extern GTextInfo **SFLookupListFromType(SplineFont *sf, int lookup_type );
extern GTextInfo **SFSubtablesOfType(SplineFont *sf, int lookup_type, int kernclass, int add_none);
extern GTextInfo *SFSubtableListOfType(SplineFont *sf, int lookup_type, int kernclass, int add_none);
extern struct lookup_subtable *SFNewLookupSubtableOfType(SplineFont *sf, int lookup_type, struct subtable_data *sd, int def_layer );
extern int EditLookup(OTLookup *otl,int isgpos,SplineFont *sf);
extern int EditSubtable(struct lookup_subtable *sub,int isgpos,SplineFont *sf,
	struct subtable_data *sd,int def_layer);
extern void _LookupSubtableContents(SplineFont *sf, struct lookup_subtable *sub,
	struct subtable_data *sd,int def_layer);
extern unichar_t **SFGlyphNameCompletion(SplineFont *sf,GGadget *t,int from_tab,
	int new_name_after_space);
extern void AddRmLang(SplineFont *sf, struct lkdata *lk,int add_lang);
extern void FVMassGlyphRename(FontView *fv);

extern void SFBdfProperties(SplineFont *sf, EncMap *map, BDFFont *thisone);



extern GMenuItem2 helplist[];
extern BasePoint last_ruler_offset[];

extern void CVCopyLayerToLayer(CharView *cv);
extern void FVCopyLayerToLayer(FontView *fv);
extern void CVCompareLayerToLayer(CharView *cv);
extern void FVCompareLayerToLayer(FontView *fv);

extern void MathInit(void);
extern void SFMathDlg(SplineFont *sf,int def_layer);

extern GMenuItem2 *cvpy_menu, *fvpy_menu;
extern void cvpy_tllistcheck(GWindow gw,struct gmenuitem *mi,GEvent *e);
extern void fvpy_tllistcheck(GWindow gw,struct gmenuitem *mi,GEvent *e);

extern void SFValidationWindow(SplineFont *sf,int layer, enum fontformat format);
extern void ValidationDestroy(SplineFont *sf);



extern const char *UI_TTFNameIds(int id);
extern const char *UI_MSLangString(int language);
extern void FontInfoInit(void);
extern void LookupUIInit(void);
extern enum psstrokeflags Ps_StrokeFlagsDlg(void);
extern struct cidmap *AskUserForCIDMap(void);

extern void DefineGroups(struct fontview *fv);
extern void DisplayGroups(struct fontview *fv);

extern struct Base *SFBaselines(SplineFont *sf,struct Base *old,int is_vertical);

extern struct hslrgb *SFFontCols(SplineFont *sf,struct hslrgb fontcols[6]);
#endif	/* _VIEWS_H */
