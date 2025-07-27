### 【1128】display temporary text in top left corner of current view 在当前视图左上角显示临时文本

#### 代码

```cpp
    /*****************************************************************************  
    **  
    ** 7174844.c  
    **  
    ** Description:  
    **     Contains Unigraphics entry points for the application.  
    **  
    *****************************************************************************/  
    /* Include files 里海译:Include files 翻译为：包含文件 */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_view.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_trns.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
        va_end(args);  
        UF_UI_open_listing_window();  
        UF_UI_write_listing_window(msg);  
        UF_print_syslog(msg, FALSE);  
    }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133];  
            UF_get_fail_message(irc, err);  
            ECHO("*** ERROR code %d at line %d in %s:\n",  
                irc, line, file);  
            ECHO("+++ %s\n", err);  
            ECHO("%s;\n", call);  
        }  
        return(irc);  
    }  
    static void map_view2abs_mod(double c[3], tag_t view)  
    {  
        char  
            vname[30];  
        double  
            abs[9] = { 0,0,0, 1,0,0, 0,1,0 },  
            vw[12] = { 0,0,0, 0,0,0, 0,0,0, 0,0,0 },  
            mx[12],  
            center[3] = { 0., 0., 0. },  
            scale = 0.;  
        int  
            irc;  
        UF_CALL(UF_OBJ_ask_name(view, vname));  
        irc = uc6430(vname, center, &scale);  
        vw[0] = center[0];  
        vw[1] = center[1];  
        vw[2] = center[2];  
        UF_CALL(uc6433(vname, &vw[3]));  
        FTN(uf5940)(vw, abs, mx, &irc);  
        FTN(uf5941)(c, mx);  
    }  
    static void RunCustomNXOpenCode(void)  
    {  
        int  
            units = -1;  
        tag_t  
            workView = NULL_TAG;  
        double  
            XYbounds[4] = { 0., 0., 0., 0. },  
            corner[3] = { 0., 0., 0. },  
            textSize=0.;  
        UF_OBJ_disp_props_t  
            dispProps;  
        dispProps.color = 186;  
        UF_CALL(UF_VIEW_ask_work_view(&workView));  
        UF_CALL(UF_VIEW_ask_current_xy_clip(workView, XYbounds));  
        corner[0] = XYbounds[0];  
        corner[1] = XYbounds[3];  
        corner[2] = 0;  
        map_view2abs_mod(corner, workView);  
        UF_CALL(UF_PART_ask_units(UF_PART_ask_display_part(),&units));  
        if(units == UF_PART_METRIC)   
        {  
            textSize = 5.;  
        }  
        else  
        {  
            textSize = .2;  
        }  
        UF_CALL(UF_DISP_display_temporary_text(workView,UF_DISP_USE_VIEW_TAG,  
            "YOUR TEXT HERE!", corner,UF_DISP_TOPLEFT, &dispProps, textSize, 1));  
    }  
    extern DllExport void ufusr( char *param, int *returnCode, int rlen )  
    {  
        if( UF_CALL(UF_initialize()) )   
        {  
            return;  
        }  
        RunCustomNXOpenCode();  
        UF_terminate();  
    }  
    extern int ufusr_ask_unload( void )  
    {  
        return( UF_UNLOAD_IMMEDIATELY );  
    }

```

#### 代码解析

> 这段代码是一个NX Open二次开发示例，主要实现了以下功能：
>
> 1. 包含了必要的NX Open头文件，用于调用NX Open的API函数。
> 2. 定义了一个错误报告函数report_error，用于在调用NX Open API失败时输出错误信息。
> 3. 定义了一个视图坐标系到绝对坐标系的转换函数map_view2abs_mod。
> 4. 定义了一个自定义函数RunCustomNXOpenCode，用于实现主体功能，包括获取当前工作视图、视图范围、视图原点等，将视图原点转换到绝对坐标系，根据当前单位设置字体大小，并在视图左上角显示自定义文字。
> 5. 定义了ufusr和ufusr_ask_unload两个导出函数，ufusr是NX Open的入口点函数，ufusr_ask_unload用于卸载插件。
> 6. ufusr函数中初始化NX Open，调用RunCustomNXOpenCode实现自定义功能，然后终止NX Open。
>
> 综上所述，这段代码通过二次开发实现了在NX Open视图中显示自定义文字的功能。
>
