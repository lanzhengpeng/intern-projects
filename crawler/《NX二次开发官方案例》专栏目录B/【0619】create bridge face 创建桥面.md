### 【0619】create bridge face 创建桥面

#### 代码

```cpp
    /*HEAD CREATE_BRIDGE_FACE CCC UFUN */  
    /* This program demonstrates a work around necessary before V12.0 to get the  
       tag of the newly created bridge face which is NOT returned by the call  
       UF_MODL_create_bridge_face.  The two numbered comments and their  
       following lines are not necessary in V12.0.  
    */  
    #include <stdio.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_disp.h>  
    #include <uf_object_types.h>  
    #include <uf_modl.h>  
    #include <uf_assem.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    static int mask_for_edges(UF_UI_selection_p_t select, void *type)  
    {  
        UF_UI_mask_t  
            mask = { UF_solid_type, 0, UF_UI_SEL_FEATURE_ANY_EDGE };  
        if (!UF_CALL(UF_UI_set_sel_mask(select,  
                UF_UI_SEL_MASK_CLEAR_AND_ENABLE_SPECIFIC, 1, &mask)))  
            return (UF_UI_SEL_SUCCESS);  
        else  
            return (UF_UI_SEL_FAILURE);  
    }  
    static tag_t select_an_edge(char *prompt)  
    {  
        int  
            resp;  
        double  
            cp[3];  
        tag_t  
            object,  
            view;  
        UF_CALL(UF_UI_select_with_single_dialog(prompt, "",  
            UF_UI_SEL_SCOPE_ANY_IN_ASSEMBLY, mask_for_edges, NULL, &resp,  
            &object, cp, &view));  
        if (resp == UF_UI_OBJECT_SELECTED || resp == UF_UI_OBJECT_SELECTED_BY_NAME)  
        {  
            UF_CALL(UF_DISP_set_highlight(object, FALSE));  
            return object;  
        }  
        else return NULL_TAG;  
    }  
    static tag_t ask_next_body(tag_t body)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            part = UF_ASSEM_ask_work_part();  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body));  
            if (body)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
                if (!subtype) return body;  
            }  
        } while (body);  
        return NULL_TAG;  
    }  
    static void do_it(void)  
    {  
        int  
            primary_dir[2] = { 1, 1 };  
        tag_t  
            bridge,  
            last,  
            next = NULL_TAG,  
            primary_edge[2],  
            primary_face[2],  
            side_edge[2] = { NULL_TAG, NULL_TAG },  
            side_face[2] = { NULL_TAG, NULL_TAG };  
        UF_STRING_t  
            side_string[2] = { { 0, NULL, NULL, NULL}, { 0, NULL, NULL, NULL }};  
        /* 1. Get the last pre-existing body first 译:获取最后一个预先存在的主体。 */  
        while ((next = ask_next_body(next)) != NULL_TAG) last = next;  
        while ((primary_face[0] = select_a_face("Select face 1")) &&  
               (primary_edge[0] = select_an_edge("Select edge 1")) &&  
               (primary_face[1] = select_a_face("Select face 2")) &&  
               (primary_edge[1] = select_an_edge("Select edge 2")))  
        {  
            UF_CALL(UF_MODL_create_bridge_face(1, 0, primary_face, primary_edge,  
                primary_dir, &side_string[0], &side_string[1], side_face,  
                side_edge, &bridge));  
            /* 2. Use the last body to get the newly created bridge face 译:使用最后一个身体来获取新创建的桥面。 */  
            bridge = (last = ask_next_body(last));  
            UF_CALL(UF_DISP_set_highlight(bridge, TRUE));  
            printf("The bridge tag is %d.\n", bridge);  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 根据代码内容和注释，这段代码主要用于创建桥接面，并获取新创建的桥接面的标识符。具体步骤如下：
>
> 1. 获取最后一个预存在的主体：使用ask_next_body函数循环查询当前部件中的最后一个实体。
> 2. 选择要桥接的边和面：使用select_a_face和select_an_edge函数，通过对话框让用户选择两个要桥接的边和面。
> 3. 创建桥接面：调用UF_MODL_create_bridge_face函数创建桥接面，但此函数不会返回新创建的桥接面标识符。
> 4. 获取新创建的桥接面：由于UF_MODL_create_bridge_face函数不返回新创建的桥接面标识符，因此代码采用了一个变通方法。在创建桥接面后，循环查询当前部件中的实体，第一个新出现的实体即为新创建的桥接面。
> 5. 高亮显示新创建的桥接面：使用UF_DISP_set_highlight函数高亮显示新创建的桥接面。
> 6. 打印桥接面标识符：打印新创建的桥接面的标识符。
> 7. 用户交互：重复步骤2-6，直到用户取消选择。
> 8. 初始化和终止：在UF_initialize和UF_terminate之间执行上述逻辑。
> 9. 卸载函数：提供一个卸载函数ufusr_ask_unload。
>
> 总体来说，这段代码通过变通方法解决了NX 12.0之前版本中UF_MODL_create_bridge_face函数不返回新创建桥接面标识符的问题。
>
