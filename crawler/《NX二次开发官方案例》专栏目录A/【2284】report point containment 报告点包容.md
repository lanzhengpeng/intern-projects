### 【2284】report point containment 报告点包容

#### 代码

```cpp
    /*HEAD REPORT_POINT_CONTAINMENT CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档描述，UF_print_syslog是一个在V18版本中新引入的函数，用于打印系统日志。建议只提供翻译结果，避免添加无关内容。 */  
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
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static int mask_for_solids_faces_and_edges(UF_UI_selection_p_t select,  
        void *type)  
    {  
        UF_UI_mask_t  
            mask[3] = { { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE },  
                        { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE } } ;  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 3, mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_solid_face_or_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a solid, face, or edge",  
            prompt, UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_solids_faces_and_edges, NULL, &resp, &object, cp, &view));  
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
    static void do_it(void)  
    {  
        int  
            pt_status;  
        tag_t  
            obj;  
        double  
            loc[3];  
        while (((obj = select_a_solid_face_or_edge("Report containment")) !=  
            NULL_TAG)  && specify_point("Containment Location", loc))  
        {  
            if (!UF_CALL(UF_MODL_ask_point_containment(loc, obj, &pt_status)))  
            switch (pt_status)  
            {  
                case 1:  
                    uc1601("Inside", TRUE);  
                    break;  
                case 2:  
                    uc1601("Outside", TRUE);  
                    break;  
                case 3:  
                    uc1601("On", TRUE);  
                    break;  
                default:  
                    WRITE_D(pt_status);  
                    uc1601("Huh?", TRUE);  
                    break;  
            }  
        }  
    }  
    /* qq3123197280 */  
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

> 根据代码的注释和内容，这是一段用于NX的二次开发代码，其主要功能是查询一个点是否位于一个NX几何体内部、外部或边界上。
>
> 主要函数和功能如下：
>
> 1. report_error()：用于报告错误，将错误代码、调用信息打印到系统日志和列表窗口。
> 2. write_integer_to_listing_window()：将整数写入列表窗口。
> 3. mask_for_solids_faces_and_edges()：设置选择掩码，用于在对话框中只允许选择实体、面和边。
> 4. select_a_solid_face_or_edge()：显示一个对话框，让用户选择一个实体、面或边，并返回其tag。
> 5. specify_point()：显示一个对话框，让用户指定一个点的坐标。
> 6. do_it()：主函数，循环让用户选择实体、面或边，然后指定一个点，查询该点是否在所选几何体内部、外部或边界上，并将结果显示在对话框中。
> 7. ufusr()：NX调用的主函数，初始化NX环境，调用do_it()，然后终止NX环境。
> 8. ufusr_ask_unload()：询问NX是否立即卸载该二次开发代码。
>
> 这段代码主要用于测试和学习NX的几何体查询功能，用户可以查询一个点与NX几何体的位置关系。
>
