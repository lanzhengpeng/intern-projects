### 【0791】create note in upper left corner of current view 在当前视图左上角创建注释

#### 代码

```cpp
    /*HEAD CREATE_NOTE_IN_UPPER_LEFT_CORNER_OF_CURRENT_VIEW CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_view.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
    #include <uf_curve.h>  
    #include <uf_csys.h>  
    #include <uf_drf.h>  
    #include <uf_vec.h>  
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    static void map_current_view2abs(double *c)  
    {  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            mx[12],  
            scale,  
            vw[12] = { 0,0,0 };  
        int  
            irc;  
        uc6430("", vw, &scale);  
        uc6433("", &vw[3]);  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void ask_current_view_corners(double corners[4][3])  
    {  
        double  
            bounds[4];  
        uc6450("", "", 0, 0);  
        UF_CALL(UF_VIEW_ask_xy_clip(NULL_TAG, bounds));  
        corners[0][0] = bounds[0];  
        corners[0][1] = bounds[2];  
        corners[0][2] = 0;  
        map_current_view2abs(&corners[0][0]);  
        corners[1][0] = bounds[1];  
        corners[1][1] = bounds[2];  
        corners[1][2] = 0;  
        map_current_view2abs(&corners[1][0]);  
        corners[2][0] = bounds[1];  
        corners[2][1] = bounds[3];  
        corners[2][2] = 0;  
        map_current_view2abs(&corners[2][0]);  
        corners[3][0] = bounds[0];  
        corners[3][1] = bounds[3];  
        corners[3][2] = 0;  
        map_current_view2abs(&corners[3][0]);  
    }  
    static tag_t set_wcs_to_current_view(void)  
    {  
        tag_t  
            csys,                       /* Csys object  译:Csys object翻译为坐标系对象。 */  
            matobj,                     /* Matrix object 译:Matrix 对象 */  
            orig_wcs;  
        double  
            origin[3],                  /* View's center 译:视图的中心 */  
            matrix[9],                  /* View's matrix 译:视图的矩阵 */  
            scale;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(uc6430("", origin, &scale));  
        UF_CALL(uc6433("", matrix));  
        if (!UF_CALL(UF_CSYS_create_matrix(matrix, &matobj)) &&  
            !UF_CALL(UF_CSYS_create_temp_csys(origin, matobj, &csys)))  
            UF_CALL(UF_CSYS_set_wcs(csys));  
        return orig_wcs;  
    }  
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    #include <uf_undo.h>  
    static void set_undo_mark(char *label)  
    {  
        UF_UNDO_mark_id_t  
            mark_id;  
        UF_CALL(UF_UNDO_set_mark(UF_UNDO_visible, label, &mark_id));  
    }  
    static void do_it(void)  
    {  
        int  
            mpi[100],  
            old_es = 1;  
        tag_t  
            note,  
            old_wcs;  
        double  
            corners[4][3],  
            mpr[100],  
            old_cs,  
            origin[3],  
            screen_height;  
        char  
            rad_dia[27],  
            *text[1] = { "UPPER LEFT" };  
        set_undo_mark("Label upper left view corner");  
        ask_current_view_corners(corners);  
        old_wcs = set_wcs_to_current_view();  
        UF_CALL(UF_CSYS_map_point(UF_CSYS_WORK_COORDS, &corners[3][0],  
            UF_CSYS_ROOT_WCS_COORDS, origin));  
        display_temporary_text("0", &corners[0][0]);  
        display_temporary_text("1", &corners[1][0]);  
        display_temporary_text("2", &corners[2][0]);  
        display_temporary_text("3", &corners[3][0]);  
        UF_VEC3_distance(&corners[0][0], &corners[3][0], &screen_height);  
        UF_CALL(UF_DRF_ask_preferences(mpi, mpr, rad_dia, rad_dia));  
        old_es = mpi[30];  
        mpi[30] = 1;  /* Entity site = upper left 译:实体站点位于左上角 */  
        old_cs = mpr[44];  
        mpr[44] = screen_height/80;  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad_dia, rad_dia));  
    /*  Adjust origin in and down 1/2 character size 译:调整原点位置向内且向下1/2字符大小。 */  
        origin[0] = origin[0] + mpr[44]/2;  
        origin[1] = origin[1] - mpr[44]/2;  
        UF_CALL(UF_DRF_create_note(1, text, origin, 0, &note));  
        mpi[30] = old_es;  
        mpr[44] = old_cs;  
        UF_CALL(UF_DRF_set_preferences(mpi, mpr, rad_dia, rad_dia));  
        UF_CALL(UF_CSYS_set_wcs(old_wcs));  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是在当前视图左上角创建一个注释。主要步骤包括：
>
> 1. 包含必要的NX Open头文件，并定义错误报告函数。
> 2. 定义函数map_current_view2abs()，用于将当前视图的坐标映射到绝对坐标系。
> 3. 定义函数ask_current_view_corners()，用于获取当前视图的四个角点坐标。
> 4. 定义函数set_wcs_to_current_view()，用于将当前视图坐标系设置为工作坐标系。
> 5. 定义函数display_temporary_text()，用于在指定位置显示临时文本。
> 6. 定义函数set_undo_mark()，用于设置撤销标记。
> 7. 在函数do_it()中，首先获取当前视图的四个角点坐标，然后设置撤销标记。
> 8. 调整视图坐标系，并在四个角点显示临时文本。
> 9. 获取屏幕高度，并设置注释的字体大小。
> 10. 创建注释并调整位置。
> 11. 恢复注释字体设置和视图坐标系。
> 12. 在ufusr()函数中，初始化NX Open，执行do_it()函数，然后终止NX Open。
> 13. 定义ufusr_ask_unload()函数，返回立即卸载标志。
>
> 整体而言，该代码实现了在当前视图左上角创建注释的功能。
>
