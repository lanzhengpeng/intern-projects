### 【3146】report if object is wave linked 检查选定的对象是否为波链接

#### 代码

```cpp
    /*HEAD REPORT_IF_OBJECT_IS_WAVE_LINKED CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_modl.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_sket.h>  
    #include <uf_so.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的说明，UF_print_syslog是V18版本中新增的功能。 */  
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
    static tag_t select_an_object(char *prompt)  
    {  
        double  
            cp[3];  
        int  
            resp;  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select an object", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, NULL,NULL,&resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_next_sketch(tag_t part, tag_t sketch)  
    {  
        UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch));  
        return (sketch);  
    }  
    static logical is_sketch_geometry(tag_t object)  
    {  
        logical  
            is_sketch_geom = FALSE;  
        int  
            ii,  
            n;  
        tag_t  
            *geoms,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        while (!is_sketch_geom &&  
               ((sketch = ask_next_sketch(part, sketch)) != NULL_TAG))  
        {  
            UF_CALL(UF_SKET_ask_geoms_of_sketch(sketch, &n, &geoms));  
            for (ii = 0; ii < n; ii++)  
                if (geoms[ii] == object)  
                {  
                    is_sketch_geom = TRUE;  
                    break;  
                }  
            if (n > 0) UF_free(geoms);  
        }  
        return is_sketch_geom;  
    }  
    static logical is_object_wave_linked(tag_t object)  
    {  
        logical  
            is_so,  
            is_wave = FALSE;  
        tag_t  
            feat = NULL_TAG;  
        char  
            *string;  
        if (!UF_MODL_ask_object_feat(object, &feat) && (feat != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_ask_feat_type(feat, &string));  
            printf("%d = %s\n", feat, string);  
            if (strstr(string, "LINK") == string) is_wave = TRUE;  
            UF_free(string);  
        }  
        else  
        {  
            UF_CALL(UF_SO_is_so(object, &is_so));  
            if (is_so && is_sketch_geometry(object)) is_wave = TRUE;  
        }  
        return is_wave;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object;  
        while ((object = select_an_object("Select an object")) != NULL_TAG)  
            if (is_object_wave_linked(object))  
                uc1601("That is wave linked", TRUE);  
            else  
                uc1601("That is NOT wave linked", TRUE);  
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

> 这段代码是用于NX的二次开发代码，其主要功能是检查用户选择的对象是否为波浪链接（wave linked）。
>
> 具体介绍如下：
>
> 1. 错误报告函数：定义了一个错误报告函数report_error，用于打印错误代码和消息。
> 2. 对象选择函数：定义了一个对象选择函数select_an_object，用于提示用户选择对象。
> 3. 草图遍历函数：定义了一个草图遍历函数ask_next_sketch，用于遍历当前部件的草图对象。
> 4. 草图几何判断函数：定义了一个草图几何判断函数is_sketch_geometry，用于判断给定的对象是否属于某个草图。
> 5. 波浪链接判断函数：定义了一个波浪链接判断函数is_object_wave_linked，用于判断给定的对象是否为波浪链接。
> 6. 主函数：定义了主函数do_it，循环提示用户选择对象，并调用判断函数检查是否为波浪链接，然后给出提示。
> 7. ufusr函数：定义了ufusr函数，用于初始化和调用主函数。
> 8. 卸载函数：定义了卸载函数ufusr_ask_unload，用于卸载二次开发代码。
>
> 总体来说，这段代码的主要功能是检测用户选择的对象是否为波浪链接，从而可以用于检查模型中的波浪链接对象。
>
