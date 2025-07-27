### 【2096】report face parms at selected points 报告选定点的面参数

#### 代码

```cpp
    /*HEAD REPORT_FACE_PARMS_AT_SELECTED_POINTS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #define ECHOF(X)   (printf("%s = %f\n", #X, X))  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog在V18中是新增的函数。 */  
            UF_print_syslog(msg, FALSE);  
            UF_print_syslog(err, FALSE);  
            UF_print_syslog("\n", FALSE);  
            UF_print_syslog(call, FALSE);  
            UF_print_syslog(";\n", FALSE);  
            if (!UF_UI_open_listing_window())  
            {  
                UF_UI_write_listing_window(msg);  
                UF_UI_write_listing_window(err);  
                UF_UI_write_listing_window("\n");  
                UF_UI_write_listing_window(call);  
                UF_UI_write_listing_window(";\n");  
            }  
        }  
        return(irc);  
    }  
    static int mask_for_faces(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical specify_point(char *prompt, double loc[3])  
    {  
        int  
            ia2[2] = { 0,0 },  
            ip3 = 0,  
            irc;  
        irc = uc1616(prompt, ia2, ip3, loc);  
        if (irc == 5) return TRUE;  
        else return FALSE;  
    }  
    static void display_temporary_point(double *coords)  
    {  
        UF_OBJ_disp_props_t  
            attrib = { 1, UF_OBJ_WHITE, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_point(NULL_TAG, UF_DISP_USE_ACTIVE_PLUS,  
            coords, &attrib, UF_DISP_POINT));  
    }  
    static void report_face_parms(tag_t face, double loc[3])  
    {  
        double  
            parms[2],  
            pnt[3];  
        UF_CALL(UF_MODL_ask_face_parm(face, loc, parms, pnt));  
        ECHOF(parms[0]);  
        ECHOF(parms[1]);  
        display_temporary_point(pnt);  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        double  
            loc[3];  
        while ((face = select_a_face("Select face")) != NULL_TAG)  
            while (specify_point("Indicate location", loc))  
                report_face_parms(face, loc);  
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

> 这段代码是一个NX的二次开发代码，主要功能是从NX中选取一个面，然后在面上指定一个点，最后报告该点的参数。
>
> 代码的主要逻辑包括：
>
> 1. 定义错误报告函数report_error，用于打印错误信息。
> 2. 定义面选择掩码函数mask_for_faces，用于在对话框中选择面时只显示实体面。
> 3. 定义选择面的函数select_a_face，用于弹出一个对话框，让用户选择一个面。
> 4. 定义指定点的函数specify_point，用于提示用户输入一个点坐标。
> 5. 定义显示临时点的函数display_temporary_point，用于在NX中显示一个临时点。
> 6. 定义报告面参数的函数report_face_parms，用于查询指定点的UV参数，并显示临时点。
> 7. 主函数do_it中循环调用select_a_face和specify_point，获取面和点，然后调用report_face_parms报告参数。
> 8. ufusr函数初始化NX环境，调用do_it，然后终止NX环境。
> 9. ufusr_ask_unload函数用于卸载用户自定义函数。
>
> 这段代码的主要流程是通过用户交互选择面和点，然后获取该点的参数，并在NX中显示出来。
>
