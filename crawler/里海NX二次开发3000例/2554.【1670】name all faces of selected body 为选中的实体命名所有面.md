### 【1670】name all faces of selected body 为选中的实体命名所有面

#### 代码

```cpp
    /*HEAD NAME_ALL_FACES_OF_SELECTED_BODY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_disp.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_ui.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 中的新功能。 */  
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
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int make_an_array(uf_list_p_t *object_list, tag_t **objects)  
    {  
        int  
            ii,  
            n;  
        uf_list_p_t  
            temp;  
        UF_CALL(UF_MODL_ask_list_count(*object_list, &n));  
        UF_CALL(allocate_memory(n * sizeof(tag_t), (void **)objects));  
        for (ii = 0, temp = *object_list; ii < n; temp = temp->next, ii++)  
            (*objects)[ii] = temp->eid;  
        UF_CALL(UF_MODL_delete_list(object_list));  
        return n;  
    }  
    static int ask_body_faces(tag_t body, tag_p_t *body_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        return make_an_array(&face_list, body_faces);  
    }  
    static void do_it(void)  
    {  
        int  
            inx,  
            count;  
        tag_t  
            body,  
            *faces;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_CALL( UF_DISP_set_name_display_status ( UF_DISP_NAME_DISPLAY_ON ));  
        while ((body = select_a_body("Select body to link")) != NULL_TAG)  
        {  
            count = ask_body_faces(body, &faces);  
            for( inx = 0; inx < count; inx++ )  
            {  
                sprintf( name, "FACE_%d", inx+1 );  
                UF_CALL( UF_OBJ_set_name( faces[inx], name ));  
            }  
            if( count > 0 ) UF_free(faces);  
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

> 这段代码的主要功能是从NX中选择一个实体，获取该实体的所有面，并为每个面设置名称。
>
> 具体来说，代码的主要流程如下：
>
> 1. 初始化：调用UF_initialize()初始化NX API。
> 2. 选择实体：调用select_a_body()函数，弹出一个对话框，让用户选择一个实体。
> 3. 获取面列表：对用户选择的实体，调用ask_body_faces()函数，获取该实体的所有面，并将这些面存储在一个数组中。
> 4. 设置面名称：遍历面数组，调用UF_OBJ_set_name()函数，为每个面设置名称，格式为"FACE_1"、"FACE_2"等。
> 5. 循环选择：循环执行步骤2-4，直到用户选择取消。
> 6. 清理：调用UF_terminate()终止NX API。
> 7. 卸载：ufusr_ask_unload()函数返回UF_UNLOAD_IMMEDIATELY，表示立即卸载该UF。
>
> 此外，代码中还包括了一些辅助函数，例如report_error()用于打印错误信息，mask_for_bodies()用于设置选择过滤器等。总体来说，这段代码实现了选择实体、获取面信息、设置面名称的功能，并提供了错误处理和内存管理。
>
