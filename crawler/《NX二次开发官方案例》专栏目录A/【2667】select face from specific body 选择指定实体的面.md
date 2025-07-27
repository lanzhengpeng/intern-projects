### 【2667】select face from specific body 选择指定实体的面

#### 代码

```cpp
    /*HEAD SELECT_FACE_FROM_SPECIFIC_BODY CCC UFUN */  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是一个在 V18 版本中新增的函数。

UF_print_syslog 是 V18 版本新增的函数。 */  
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
    /* qq3123197280 */  
    static int filter_body_faces(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        tag_t  
            face_body,  
            the_body = *(tag_t *)data;  
        UF_CALL(UF_MODL_ask_face_body(object, &face_body));  
        if (face_body == the_body)  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* qq3123197280 */  
    static int mask_for_body_faces(UF_UI_selection_p_t select, void *data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_body_faces, NULL, data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_face_from_body(char *prompt, tag_t body)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select body face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_body_faces, &body, &resp, &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        return NULL_TAG;  
    }  
    static logical is_item_on_list(uf_list_p_t object_list, tag_t item)  
    {  
        int  
            ii;  
        uf_list_p_t  
            temp;  
        for (ii = 0, temp = object_list; temp != NULL; temp = temp->next, ii++)  
            if (temp->eid == item) return TRUE;  
        return FALSE;  
    }  
    /* qq3123197280 */  
    static int filter_list_objects(tag_t object, int type[3], void *data,  
        UF_UI_selection_p_t select)  
    {  
        uf_list_p_t  
            the_list = (uf_list_p_t)data;  
        if (is_item_on_list(the_list, object))  
            return (UF_UI_SEL_ACCEPT);  
        else  
            return (UF_UI_SEL_REJECT);  
    }  
    /* qq3123197280 */  
    static int mask_for_face_in_list(UF_UI_selection_p_t select, void *data)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_FACE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)) &&  
            !UF_CALL(UF_UI_set_sel_procs(select, filter_list_objects, NULL, data)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_face_in_list(char *prompt, uf_list_p_t list)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog("Select list face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY,  
            mask_for_face_in_list, list, &resp, &object, cp, &view));  
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
            body,  
            face;  
        uf_list_p_t  
            face_list;  
        while ((body = select_a_body("Select Body")) != NULL_TAG)  
        {  
        /*  Method 1 里海译:Method 1:

1. 准备材料：新鲜柠檬汁、白醋、食盐和温开水。
2. 将新鲜柠檬汁、白醋、食盐按照1:1:1的比例混合。
3. 将混合液倒入温开水中，搅拌均匀。
4. 用调配好的液体擦拭家具表面，然后用干布擦干。
5. 这种方法不仅可以去除家具表面的污渍，还可以起到消毒和提亮的作用。 */  
            face = select_face_from_body("Select face on that body", body);  
        /*  Method 2 里海译:好的，这是Method 2的翻译：

Method 2

步骤1: 将问题分解成小部分

将复杂问题分解成多个小问题，然后逐个解决。这种方法可以帮助你更有效地解决问题。

步骤2: 使用图表和图形

图表和图形可以帮助你更直观地理解问题，使复杂问题更容易解决。

步骤3: 寻求帮助

当你遇到难题时，寻求他人的帮助可以提供新的视角，并帮助你找到更好的解决方案。

步骤4: 使用模型和公式

模型和公式可以提供解决问题的框架，使你能够系统地解决问题。

步骤5: 持之以恒

解决问题需要时间和努力，持之以恒是成功的关键。不要放弃，继续努力直到找到解决方案。

步骤6: 复习和总结

在解决问题后，回顾和总结你的方法可以帮助你在未来更好地解决问题。

步骤7: 分享你的经验

分享你的问题和解决方案可以让你从他人的经验中学习，并提高你的问题解决能力。

步骤8: 练习和练习

练习可以帮助你提高问题解决技能，使你在未来遇到问题时更加从容应对。 */  
            UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
            face = select_face_in_list("Select face on that body", face_list);  
            UF_CALL(UF_MODL_delete_list(&face_list));  
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

> 这是NX二次开发代码，其主要功能是从特定的实体上选择一个面。
>
> 代码的主要部分包括：
>
> 1. 错误处理函数：用于记录UF函数调用错误信息。
> 2. 函数mask_for_bodies()：设置选择模式，只允许选择实体。
> 3. 函数select_a_body()：用于选择一个实体。
> 4. 函数filter_body_faces()：用于过滤实体，确保只选择特定实体的面。
> 5. 函数mask_for_body_faces()：设置选择模式，只允许选择特定实体的面。
> 6. 函数select_face_from_body()：用于选择特定实体的一个面。
> 7. 函数is_item_on_list()：用于检查面是否在列表中。
> 8. 函数filter_list_objects()：用于过滤不在列表中的面。
> 9. 函数mask_for_face_in_list()：设置选择模式，只允许选择列表中的面。
> 10. 函数select_face_in_list()：用于从列表中选择一个面。
> 11. 函数do_it()：主函数，先选择一个实体，然后选择该实体上的一个面，最后从该实体的面列表中选择一个面。
> 12. ufusr()：NX二次开发主函数。
> 13. ufusr_ask_unload()：卸载函数。
>
> 总体来说，这段代码实现了从特定实体上选择面的功能，包括选择实体、选择实体的面、获取实体面的列表、从列表中选择面等功能。
>
