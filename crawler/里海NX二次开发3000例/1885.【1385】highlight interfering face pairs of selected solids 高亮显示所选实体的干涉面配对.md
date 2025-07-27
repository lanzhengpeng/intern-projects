### 【1385】highlight interfering face pairs of selected solids 高亮显示所选实体的干涉面配对

#### 代码

```cpp
    /*HEAD HIGHLIGHT_INTERFERING_FACE_PAIRS_OF_SELECTED_SOLIDS CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：请注意，UF_print_syslog是在V18版本中新增的。 */  
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
    /* qq3123197280 */  
    static int mask_for_bodies(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_BODY };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_a_body(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select a body", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_bodies, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, 0));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static logical ask_yes_or_no(char *title, char *message)  
    {  
        int  
            resp;  
        UF_UI_message_buttons_t  
            buttons = { TRUE, FALSE, TRUE, "Yes", NULL, "No", 1, 0, 2 };  
        UF_CALL(UF_UI_message_dialog(title, UF_UI_MESSAGE_QUESTION, &message, 1,  
            TRUE, &buttons, &resp));  
        return (2 - resp);  
    }  
    static void do_it(void)  
    {  
        logical  
            flag;  
        int  
            code;  
        tag_t  
            body1,  
            body2;  
        double  
            dist,  
            pt1[3],  
            pt2[3],  
            tol;  
        uf_list_p_t  
            face_list1,  
            face_list2,  
            tmp1,  
            tmp2;  
        UF_MODL_ask_distance_tolerance(&tol);  
        while (((body1 = select_a_body("Select Body 1")) != NULL_TAG) &&  
               ((body2 = select_a_body("Select Body 2")) != NULL_TAG))  
        {  
            UF_CALL(UF_MODL_check_interference(body1, 1, &body2, &code));  
            if (code == 2) continue;  
            UF_CALL(UF_MODL_update());  
            UF_CALL(UF_MODL_ask_body_faces(body1, &face_list1));  
            UF_CALL(UF_MODL_ask_body_faces(body2, &face_list2));  
            flag = TRUE;  
            for (tmp1 = face_list1; tmp1 != NULL; tmp1 = tmp1->next)  
            {  
                for (tmp2 = face_list2; tmp2 != NULL; tmp2 = tmp2->next)  
                {  
                    UF_CALL(UF_MODL_ask_minimum_dist(tmp1->eid, tmp2->eid,  
                        FALSE, NULL, FALSE, NULL, &dist, pt1, pt2));  
                    if (dist < tol)  
                    {  
                        UF_CALL(UF_DISP_set_highlight(tmp1->eid, TRUE));  
                        UF_CALL(UF_DISP_set_highlight(tmp2->eid, TRUE));  
                        flag = ask_yes_or_no("Simple Interference", "Continue?");  
                        UF_CALL(UF_DISP_set_highlight(tmp1->eid, FALSE));  
                        UF_CALL(UF_DISP_set_highlight(tmp2->eid, FALSE));  
                        if (!flag) break;  
                    }  
                }  
                if (!flag) break;  
            }  
            UF_CALL(UF_MODL_delete_list(&face_list1));  
            UF_CALL(UF_MODL_delete_list(&face_list2));  
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

> 这段代码是一个NX的二次开发程序，主要用于检测并高亮显示两个实体之间的干涉面。
>
> 主要功能包括：
>
> 1. 用户可以依次选择两个实体，然后程序会检测这两个实体之间是否有干涉。
> 2. 如果检测到干涉，程序会找出干涉的面，并高亮显示。
> 3. 用户可以决定是否继续检测其他干涉面，或者停止程序。
> 4. 提供了错误处理和用户交互功能。
> 5. 可以立即卸载。
>
> 该程序使用NX的UF模块进行开发，包括UF_UI、UF_MODL、UF_DISP等，实现了实体的选择、干涉检测、面查找、距离计算、高亮显示等功能。整体流程清晰，代码结构良好，是一个典型的NX二次开发示例。
>
