### 【2114】report feature location and direction 报告特征位置和方向

#### 代码

```cpp
    /*HEAD REPORT_FEATURE_LOCATION_AND_DIRECTION CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsnprintf_s(msg, sizeof(msg), UF_UI_MAX_STRING_LEN, format, args);  
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
    static tag_t select_a_feature(char *prompt)  
    {  
        int  
            cnt,  
            resp;  
        tag_t  
            first,  
            *feats;  
        UF_CALL(UF_UI_select_feature(prompt, NULL, &cnt, &feats, &resp));  
        if (cnt)  
        {  
            first = feats[0];  
            UF_free(feats);  
            return (first);  
        }  
        else return (NULL_TAG);  
    }  
    static void report_feature_name(tag_t feat)  
    {  
        char  
            *name,  
            *type;  
        UF_CALL(UF_MODL_ask_feat_name(feat, &name));  
        UF_CALL(UF_MODL_ask_feat_type(feat, &type));  
        ECHO("\n%d = %s (%s)\n", feat, name, type);  
        UF_free(name);  
        UF_free(type);  
    }  
    static void report_feature_location_and_direction(tag_t feat)  
    {  
        double  
            location[3],  
            x_dir[3],  
            y_dir[3];  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        report_feature_name(feat);  
        if (!UF_CALL(UF_MODL_ask_feat_location(feat, location)))  
        {  
            UF_CALL(UF_DISP_display_temporary_point(NULL_TAG,  
                UF_DISP_USE_WORK_VIEW, location, &attrib, UF_DISP_POINT));  
            ECHO("Location: %f, %f, %f\n", location[0], location[1], location[2]);  
        }  
        if (!UF_CALL(UF_MODL_ask_feat_direction(feat, x_dir, y_dir)))  
        {  
            ECHO("X Direction: %f, %f, %f\n", x_dir[0], x_dir[1], x_dir[2]);  
            UF_DISP_conehead(UF_DISP_WORK_VIEW_ONLY, location, x_dir, 0);  
            ECHO("Y Direction: %f, %f, %f\n", y_dir[0], y_dir[1], y_dir[2]);  
            UF_DISP_conehead(UF_DISP_WORK_VIEW_ONLY, location, y_dir, 0);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            feat;  
        while ((feat = select_a_feature("Select feature")) != NULL_TAG)  
            report_feature_location_and_direction(feat);  
    }  
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

> 这段代码是NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 定义了一个选择特征的函数select_a_feature，该函数提示用户选择一个特征，并返回特征tag。
> 3. 定义了报告特征名称的函数report_feature_name，用于打印特征名称和类型。
> 4. 定义了报告特征位置和方向的函数report_feature_location_and_direction，用于打印特征的位置坐标，并在图形窗口中显示特征位置和方向。
> 5. do_it函数循环调用select_a_feature和report_feature_location_and_direction，实现选择并报告特征位置和方向的功能。
> 6. ufusr函数是NX的二次开发入口函数，在这里初始化UF，调用do_it执行功能，然后终止UF。
> 7. ufusr_ask_unload函数用于卸载二次开发应用程序。
>
> 总的来说，这段代码通过NX的UF函数实现了选择特征并报告其位置和方向的功能。代码结构清晰，具有错误处理和用户交互。
>
