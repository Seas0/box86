#if !(defined(GO) && defined(GOM) && defined(GO2) && defined(DATA))
#error Meh....
#endif

GO(wgetch_events, iFpp)
GO(wgetnstr_events, iFppip)
GO(addch, iFL)
GO(addchnstr, iFpi)
GO(addchstr, iFp)
GO(addnstr, iFpi)
GO(addstr, iFp)
GO(attroff, iFi)
GO(attron, iFi)
GO(attrset, iFi)
GO(attr_get, iFppp)
GO(attr_off, iFLp)
GO(attr_on, iFLp)
GO(attr_set, iFLwp)
GO(beep, iFv)
GO(bkgd, iFL)
GO(bkgdset, vFL)
GO(border, iFLLLLLLLL)
GO(box, iFpLL)
GO(can_change_color, iFv)
GO(chgat, iFiLwp)
GO(clear, iFv)
GO(clearok, iFpi)
GO(clrtobot, iFv)
GO(clrtoeol, iFv)
GO(color_content, iFwppp)
GO(color_set, iFwp)
GO(COLOR_PAIR, iFi)
GO(copywin, iFppiiiiiii)
GO(delch, iFv)
GO(delscreen, vFp)
GO(delwin, iFp)
GO(deleteln, iFv)
GO(derwin, pFpiiii)
GO(doupdate, iFv)
GO(dupwin, pFp)
GO(echo, iFv)
GO(echochar, iFL)
GO(erase, iFv)
GO(endwin, iFv)
GO(filter, vFv)
GO(flash, iFv)
GO(getbkgd, LFp)
GO(getch, iFv)
GO(getnstr, iFpi)
GO(getstr, iFp)
GO(getwin, pFp)
GO(has_colors, iFv)
GO(hline, iFLi)
GO(immedok, vFpi)
GO(inch, LFv)
GO(inchnstr, iFpi)
GO(inchstr, iFp)
GOM(initscr, pFv)
GO(init_color, iFwwww)
GO(init_pair, iFwww)
GO(innstr, iFpi)
GO(insch, iFL)
GO(insdelln, iFi)
GO(insertln, iFv)
GO(insnstr, iFpi)
GO(insstr, iFp)
GO(instr, iFp)
GO(isendwin, iFv)
GO(is_linetouched, iFpi)
GO(is_wintouched, iFp)
GO(leaveok, iFpi)
GO(move, iFii)
GO(mvaddch, iFiiL)
GO(mvaddchnstr, iFiipi)
GO(mvaddchstr, iFiip)
GO(mvaddnstr, iFiipi)
GO(mvaddstr, iFiip)
GO(mvchgat, iFiiiLwp)
GO(mvcur, iFiiii)
GO(mvdelch, iFii)
GO(mvderwin, iFpii)
GO(mvgetch, iFii)
GO(mvgetnstr, iFiipi)
GO(mvgetstr, iFiip)
GO(mvhline, iFiiLi)
GO(mvinch, LFii)
GO(mvinchnstr, iFiipi)
GO(mvinchstr, iFiip)
GO(mvinnstr, iFiipi)
GO(mvinsch, iFiiL)
GO(mvinsnstr, iFiipi)
GO(mvinsstr, iFiip)
GO(mvinstr, iFiip)
//GO(mvprintw, i(i,i,p,...)
//GO(mvscanw, i(i,i,p,...)
GO(mvvline, iFiiLi)
GO(mvwaddch, iFpiiL)
GO(mvwaddchnstr, iFpiipi)
GO(mvwaddchstr, iFpiip)
GO(mvwaddnstr, iFpiipi)
GO(mvwaddstr, iFpiip)
GO(mvwchgat, iFpiiiLwp)
GO(mvwdelch, iFpii)
GO(mvwgetch, iFpii)
GO(mvwgetnstr, iFpiipi)
GO(mvwgetstr, iFpiip)
GO(mvwhline, iFpiiLi)
GO(mvwin, iFpii)
GO(mvwinch, LFpii)
GO(mvwinchnstr, iFpiipi)
GO(mvwinchstr, iFpiip)
GO(mvwinnstr, iFpiipi)
GO(mvwinsch, iFpiiL)
GO(mvwinsnstr, iFpiipi)
GO(mvwinsstr, iFpiip)
GO(mvwinstr, iFpiip)
GOM(mvwprintw, iFEpiipV)
//GO(mvwscanw, i(p,i,i,p,...)
GO(mvwvline, iFpiiLi)
GO(newpad, pFii)
GO(newterm, pFppp)
GO(newwin, pFiiii)
GO(nl, iFv)
GO(noecho, iFv)
GO(nonl, iFv)
GO(overlay, iFpp)
GO(overwrite, iFpp)
GO(pair_content, iFwpp)
GO(PAIR_NUMBER, iFi)
GO(pechochar, iFpL)
GO(pnoutrefresh, iFpiiiiii)
GO(prefresh, iFpiiiiii)
GOM(printw, iFEpV)
GO(putwin, iFpp)
GO(redrawwin, iFp)
GO(refresh, iFv)
//GOM(ripoffline, int(int,int (*)(WINDOW*,int)))
//GO(scanw, i(p,...)
GO(scr_dump, iFp)
GO(scr_init, iFp)
GO(scrl, iFi)
GO(scroll, iFp)
GO(scrollok, iFpi)
GO(scr_restore, iFp)
GO(scr_set, iFp)
GO(setscrreg, iFii)
GO(set_term, pFp)
GO(slk_attroff, iFL)
GO(slk_attr_off, iFLp)
GO(slk_attron, iFL)
GO(slk_attr_on, iFLp)
GO(slk_attrset, iFL)
GO(slk_attr, LFv)
GO(slk_attr_set, iFLwp)
GO(slk_clear, iFv)
GO(slk_color, iFw)
GO(slk_init, iFi)
GO(slk_label, pFi)
GO(slk_noutrefresh, iFv)
GO(slk_refresh, iFv)
GO(slk_restore, iFv)
GO(slk_set, iFipi)
GO(slk_touch, iFv)
GO(standout, iFv)
GO(standend, iFv)
GO(start_color, iFv)
GO(subpad, pFpiiii)
GO(subwin, pFpiiii)
GO(syncok, iFpi)
GO(termattrs, LFv)
GO(timeout, vFi)
GO(touchline, iFpii)
GO(touchwin, iFp)
GO(ungetch, iFi)
GO(untouchwin, iFp)
GO(use_tioctl, vFi)
GO(vidattr, iFL)
//GOM(vidputs, iFLP)
GO(vline, iFLi)
GO(vwprintw, iFppp)
GO(vw_printw, iFppp)
GO(vwscanw, iFppp)
GO(vw_scanw, iFppp)
GO(waddch, iFpL)
GO(waddchnstr, iFppi)
GO(waddchstr, iFpp)
GO(waddnstr, iFppi)
GO(waddstr, iFpp)
GO(wattron, iFpi)
GO(wattroff, iFpi)
GO(wattrset, iFpi)
GO(wattr_get, iFpppp)
GO(wattr_on, iFpLp)
GO(wattr_off, iFpLp)
GO(wattr_set, iFpLwp)
GO(wbkgd, iFpL)
GO(wbkgdset, vFpL)
GO(wborder, iFpLLLLLLLL)
GO(wchgat, iFpiLwp)
GO(wclear, iFp)
GO(wclrtobot, iFp)
GO(wclrtoeol, iFp)
GO(wcolor_set, iFpwp)
GO(wcursyncup, vFp)
GO(wdelch, iFp)
GO(wdeleteln, iFp)
GO(wechochar, iFpL)
GO(werase, iFp)
GO(wgetch, iFp)
GO(wgetnstr, iFppi)
GO(wgetstr, iFpp)
GO(whline, iFpLi)
GO(winch, LFp)
GO(winchnstr, iFppi)
GO(winchstr, iFpp)
GO(winnstr, iFppi)
GO(winsch, iFpL)
GO(winsdelln, iFpi)
GO(winsertln, iFp)
GO(winsnstr, iFppi)
GO(winsstr, iFpp)
GO(winstr, iFpp)
GO(wmove, iFpii)
GO(wnoutrefresh, iFp)
//GO(wprintw, i(p,p,...)
GO(wredrawln, iFpii)
GO(wrefresh, iFp)
//GO(wscanw, i(p,p,...)
GO(wscrl, iFpi)
GO(wsetscrreg, iFpii)
GO(wstandout, iFp)
GO(wstandend, iFp)
GO(wsyncdown, vFp)
GO(wsyncup, vFp)
GO(wtouchln, iFpiii)
GO(wvline, iFpLi)
GO(getattrs, iFp)
GO(getcurx, iFp)
GO(getcury, iFp)
GO(getbegx, iFp)
GO(getbegy, iFp)
GO(getmaxx, iFp)
GO(getmaxy, iFp)
GO(getparx, iFp)
GO(getpary, iFp)
GO(is_term_resized, iFii)
GO(alloc_pair, iFii)
GO(assume_default_colors, iFii)
GO(extended_color_content, iFippp)
GO(extended_pair_content, iFipp)
GO(extended_slk_color, iFi)
GO(find_pair, iFii)
GO(free_pair, iFi)
GO(get_escdelay, iFv)
GO(init_extended_color, iFiiii)
GO(init_extended_pair, iFiii)
GO(reset_color_pairs, vFv)
GO(resize_term, iFii)
GO(resizeterm, iFii)
GO(set_escdelay, iFi)
GO(use_default_colors, iFv)
GO(use_legacy_coding, iFi)
//GOM(use_screen, iFpPp)
//GOM(use_window, iFpPp)
GO(wresize, iFpii)
GO(nofilter, vFv)
GO(wgetparent, pFp)
GO(is_cleared, iFp)
GO(is_idcok, iFp)
GO(is_idlok, iFp)
GO(is_immedok, iFp)
GO(is_keypad, iFp)
GO(is_leaveok, iFp)
GO(is_nodelay, iFp)
GO(is_notimeout, iFp)
GO(is_pad, iFp)
GO(is_scrollok, iFp)
GO(is_subwin, iFp)
GO(is_syncok, iFp)
GO(wgetdelay, iFp)
GO(wgetscrreg, iFppp)
GO(add_wch, iFp)
GO(add_wchnstr, iFpi)
GO(add_wchstr, iFp)
GO(addnwstr, iFpi)
GO(addwstr, iFp)
GO(bkgrnd, iFp)
GO(bkgrndset, vFp)
GO(border_set, iFpppppppp)
GO(box_set, iFppp)
GO(echo_wchar, iFp)
GO(erasewchar, iFp)
GO(get_wch, iFp)
GO(get_wstr, iFp)
GO(getbkgrnd, iFp)
GO(getcchar, iFppppp)
GO(getn_wstr, iFpi)
GO(hline_set, iFpi)
GO(in_wch, iFp)
GO(in_wchnstr, iFpi)
GO(in_wchstr, iFp)
GO(innwstr, iFpi)
GO(ins_nwstr, iFpi)
GO(ins_wch, iFp)
GO(ins_wstr, iFp)
GO(inwstr, iFp)
GO(key_name, pFu)
GO(killwchar, iFp)
GO(mvadd_wch, iFiip)
GO(mvadd_wchnstr, iFiipi)
GO(mvadd_wchstr, iFiip)
GO(mvaddnwstr, iFiipi)
GO(mvaddwstr, iFiip)
GO(mvget_wch, iFiip)
GO(mvget_wstr, iFiip)
GO(mvgetn_wstr, iFiipi)
GO(mvhline_set, iFiipi)
GO(mvin_wch, iFiip)
GO(mvin_wchnstr, iFiipi)
GO(mvin_wchstr, iFiip)
GO(mvinnwstr, iFiipi)
GO(mvins_nwstr, iFiipi)
GO(mvins_wch, iFiip)
GO(mvins_wstr, iFiip)
GO(mvinwstr, iFiip)
GO(mvvline_set, iFiipi)
GO(mvwadd_wch, iFpiip)
GO(mvwadd_wchnstr, iFpiipi)
GO(mvwadd_wchstr, iFpiip)
GO(mvwaddnwstr, iFpiipi)
GO(mvwaddwstr, iFpiip)
GO(mvwget_wch, iFpiip)
GO(mvwget_wstr, iFpiip)
GO(mvwgetn_wstr, iFpiipi)
GO(mvwhline_set, iFpiipi)
GO(mvwin_wch, iFpiip)
GO(mvwin_wchnstr, iFpiipi)
GO(mvwin_wchstr, iFpiip)
GO(mvwinnwstr, iFpiipi)
GO(mvwins_nwstr, iFpiipi)
GO(mvwins_wch, iFpiip)
GO(mvwins_wstr, iFpiip)
GO(mvwinwstr, iFpiip)
GO(mvwvline_set, iFpiipi)
GO(pecho_wchar, iFpp)
GO(setcchar, iFppLwp)
GO(slk_wset, iFipi)
GO(term_attrs, LFv)
GO(unget_wch, iFu)
GO(vid_attr, iFLwp)
//GOM(vid_puts, iFLwpP)
GO(vline_set, iFpi)
GO(wadd_wch, iFpp)
GO(wadd_wchnstr, iFppi)
GO(wadd_wchstr, iFpp)
GO(waddnwstr, iFppi)
GO(waddwstr, iFpp)
GO(wbkgrnd, iFpp)
GO(wbkgrndset, vFpp)
GO(wborder_set, iFppppppppp)
GO(wecho_wchar, iFpp)
GO(wget_wch, iFpp)
GO(wget_wstr, iFpp)
GO(wgetbkgrnd, iFpp)
GO(wgetn_wstr, iFppi)
GO(whline_set, iFppi)
GO(win_wch, iFpp)
GO(win_wchnstr, iFppi)
GO(win_wchstr, iFpp)
GO(winnwstr, iFppi)
GO(wins_nwstr, iFppi)
GO(wins_wch, iFpp)
GO(wins_wstr, iFpp)
GO(winwstr, iFpp)
GO(wunctrl, pFp)
GO(wvline_set, iFppi)
GO(term_attrs, LFp)
GO(unget_wch, iFpu)
GO(vid_attr, iFpLwp)
//GOM(vid_puts, iFpLwpP)
GO(_nc_viswbuf, pFp)
GO(_nc_viswibuf, pFp)
GO(has_mouse, iFv)
GO(getmouse, iFp)
GO(ungetmouse, iFp)
GO(mousemask, LFLp)
GO(wenclose, iFpii)
GO(mouseinterval, iFi)
GO(wmouse_trafo, iFpppi)
GO(mouse_trafo, iFppi)
GO(has_mouse, iFp)
GO(getmouse, iFpp)
GO(ungetmouse, iFpp)
GO(mousemask, LFpLp)
GO(mouseinterval, iFpi)
GO(mcprint, iFpi)
GO(mcprint, iFppi)
//GO(_tracef, v(p,...) GCC_PRINTFLIKE(1,2)
GO(_traceattr, pFL)
GO(_traceattr2, pFiL)
GO(_tracechar, pFi)
GO(_tracechtype, pFL)
GO(_tracechtype2, pFiL)
GO(_tracecchar_t, pFp)
GO(_tracecchar_t2, pFip)
GO(trace, vFu)
GO(_nc_visbuf, pFp)

DATA(COLOR_PAIRS, 4)
DATA(COLS, 4)
DATA(COLORS, 4)
DATA(ESCDELAY, 4)
DATA(LINES, 4)
DATA(TABSIZE, 4)
DATA(curscr, 4)
DATA(newscr, 4)
DATA(stdscr, 4)
DATA(ttytype, 4)
DATA(_nc_wacs, 4)
