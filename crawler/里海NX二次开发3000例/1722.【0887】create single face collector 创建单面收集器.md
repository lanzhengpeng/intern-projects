### 【0887】create single face collector 创建单面收集器

#### 代码

```cpp
    /*HEAD CREATE_SINGLE_FACE_COLLECTOR CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_sc.h>  
    #include <uf_sc_type.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据原文内容，UF_print_syslog 是在 V18 版本中新增的。 */  
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
    static tag_t create_single_face_collector(tag_t face, tag_t obj_in_part)  
    {  
        tag_t  
            dumbfaces[1],  
            face_cntr;  
        int  
            rule_types[1];  
        UF_SC_input_data_t  
            rules [1];  
        UF_SC_FACE_dumb_data_t  
            dumb_face_data ;  
        rule_types[0] = UF_SC_FACE_DUMB_FACES;  
        dumbfaces[0] = face;  
        dumb_face_data.num_faces = 1 ;  
        dumb_face_data.faces = dumbfaces;  
        rules[0].face_dumb_input = dumb_face_data ;  
        if (!UF_CALL(UF_MODL_create_smart_face_container(  
                obj_in_part, 1, rule_types, rules, &face_cntr)))  
            return face_cntr;  
        else  
            return NULL_TAG;  
    }  
    /* 里海 */  
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
        UF_CALL(UF_UI_select_with_single_dialog("Select a face", prompt,  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_faces, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        tag_t  
            object,  
            face_container;  
        char  
            msg[133];  
        if ((object = select_a_face("Select face")) != NULL_TAG)  
        {  
            face_container = create_single_face_collector(object, object);  
            sprintf(msg, "Face Collector Tag: %d", face_container);  
            uc1601(msg, TRUE);  
        }  
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

> 这段代码是一个NX二次开发示例，用于创建一个包含单个面的面收集器(face collector)。
>
> 主要功能包括：
>
> 1. 错误报告函数：report_error，用于在发生UF函数调用错误时打印错误信息。
> 2. 创建面收集器函数：create_single_face_collector，用于创建一个包含指定面的面收集器。
> 3. 面选择函数：mask_for_faces和select_a_face，用于提示用户选择一个面。
> 4. 主函数：do_it，用于调用面选择函数获取一个面，然后调用创建面收集器函数创建面收集器。
> 5. UF初始化/终止函数：ufusr，用于在NX启动时自动调用，初始化UF，调用主函数，然后终止UF。
> 6. 卸载函数：ufusr_ask_unload，用于在NX卸载时自动调用，立即卸载二次开发代码。
>
> 通过这些函数，代码实现了在NX中创建一个包含用户选择的一个面的面收集器的功能。
>
