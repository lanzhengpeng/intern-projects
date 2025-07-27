### 【2700】select round planar faces 选择圆形平面面

#### 代码

```cpp
    /*HEAD SELECT_ROUND_PLANAR_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：

注意：UF_print_syslog是在V18中新增的。 */  
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
    static tag_t ask_face_first_outer_edge(tag_t face)  
    {  
        tag_t  
            edge;  
        uf_loop_p_t  
            loop_list,  
            temp;  
        if (!UF_CALL(UF_MODL_ask_face_loops(face, &loop_list)) && loop_list)  
        {  
            for (temp = loop_list; temp != NULL; temp = temp->next)  
                if (temp->type == 1)  
                {  
                    edge = temp->edge_list->eid;  
                    break;  
                }  
            UF_CALL(UF_MODL_delete_loop_list(&loop_list));  
            return edge;  
        }  
        else  
            return NULL_TAG;  
    }  
    /* qq3123197280 */  
    static int filter_round_faces(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        int  
            face_type;  
        tag_t  
            edge = ask_face_first_outer_edge(object);  
        UF_CALL(UF_MODL_ask_edge_type(edge, &face_type));  
        if (face_type == UF_MODL_CIRCULAR_EDGE)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* qq3123197280 */  
    static int mask_for_round_planar_face(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_PLANAR_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_round_faces, NULL, NULL)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_round_planar_face(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select planar face", prompt,  
            UF_UI_SEL_SCOPE_WORK_PART,  
            mask_for_round_planar_face, NULL, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            face;  
        while ((face = select_round_planar_face("Round/Flat face")) != NULL_TAG);  
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

> 这段代码是用于NX的二次开发，主要功能是选择圆形平面面。以下是代码的详细说明：
>
> 1. 头文件包含：代码中包含了必要的头文件，如stdio.h、string.h、uf.h等，以调用NX提供的API。
> 2. 错误处理函数：定义了一个错误处理函数report_error，用于在调用NX API失败时打印错误信息。
> 3. 获取面第一个外边函数：定义了一个函数ask_face_first_outer_edge，用于获取面的第一个外边。
> 4. 过滤圆形面函数：定义了一个函数filter_round_faces，用于在用户选择过程中过滤出圆形面。
> 5. 设置选择面类型掩码函数：定义了一个函数mask_for_round_planar_face，用于设置选择面的类型掩码。
> 6. 选择圆形平面面函数：定义了一个函数select_round_planar_face，用于提示用户选择圆形平面面。
> 7. 主函数：定义了一个主函数do_it，循环调用select_round_planar_face函数让用户选择圆形平面面。
> 8. ufusr函数：NX二次开发的入口函数，初始化NX，调用主函数，然后结束NX。
> 9. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载用户程序时调用。
>
> 总的来说，这段代码通过二次开发实现了让用户选择圆形平面面的功能。
>
