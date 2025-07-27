### 【2883】show selected features faces ids 显示所选特征面ID

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <stdarg.h>  
    static void ECHO(char *format, ...)  
    {  
        char msg[UF_UI_MAX_STRING_LEN+1];  
        va_list args;  
        va_start(args, format);  
        vsprintf(msg, format, args);  
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
    static void display_temporary_text(char *text, double loc[3])  
    {  
        UF_OBJ_disp_props_t  
            props = { 1, 0, UF_OBJ_NOT_BLANKED, UF_OBJ_WIDTH_NORMAL,  
                UF_OBJ_FONT_SOLID, FALSE};  
        UF_CALL(UF_DISP_display_temporary_text(NULL_TAG, UF_DISP_USE_WORK_VIEW,  
            text, loc, UF_DISP_MIDDLECENTER, &props, 0, 0));  
    }  
    static int ask_object_id(tag_t object)  
    {  
        unsigned int  
            sub_id,  
            ver;  
        char  
            *file_data,  
            *handle;  
        handle = UF_TAG_ask_handle_of_tag(object);  
        UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &sub_id, &ver));  
        UF_free(handle);  
        UF_free(file_data);  
        return sub_id;  
    }  
    static void number_objects_with_ids(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d - %d", ii+1, ask_object_id(objects[ii]));  
            if (UF_OBJ_ask_name_origin(objects[ii], loc))  
            {  
                UF_CALL(UF_OBJ_set_name(objects[ii], "X"));  
                UF_CALL(UF_OBJ_ask_name_origin(objects[ii], loc));  
                UF_CALL(UF_OBJ_delete_name(objects[ii]));  
            }  
            display_temporary_text(msg, loc);  
        }  
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
    static int ask_feature_faces(tag_t feature, tag_p_t *feature_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_feat_faces(feature, &face_list));  
        return make_an_array(&face_list, feature_faces);  
    }  
    static void do_it(void)  
    {  
        int  
            n_faces;  
        tag_t  
            feature,  
            *faces;  
        while ((feature = select_a_feature("Number faces with IDs")) != NULL_TAG)  
        {  
            n_faces = ask_feature_faces(feature, &faces);  
            number_objects_with_ids(faces, n_faces);  
            if (n_faces > 0) UF_free(faces);  
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

> 这段NX二次开发代码主要实现了以下功能：
>
> 1. 定义了一个ECHO宏，用于输出信息到NX的日志窗口和系统日志。
> 2. 定义了一个report_error函数，用于在函数调用出错时输出错误信息。
> 3. 定义了一个select_a_feature函数，用于让用户选择一个特征，并返回特征标签。
> 4. 定义了一个display_temporary_text函数，用于在指定的位置显示临时文本。
> 5. 定义了一个ask_object_id函数，用于获取一个对象的子ID。
> 6. 定义了一个number_objects_with_ids函数，用于给一组对象编号并显示编号。
> 7. 定义了一个allocate_memory函数，用于分配内存。
> 8. 定义了一个make_an_array函数，用于将链表转换成数组。
> 9. 定义了一个ask_feature_faces函数，用于获取一个特征的所有面，并返回面标签数组。
> 10. 定义了一个do_it函数，用于循环让用户选择特征，获取特征的所有面，并为这些面编号显示。
> 11. 定义了ufusr函数，作为NX的入口函数，初始化后调用do_it函数，最后终止。
> 12. 定义了ufusr_ask_unload函数，用于设置立即卸载。
>
> 综合来看，这段代码主要用于获取用户选择的一个特征的所有面，并为这些面编号显示，通过二次开发扩展了NX的功能。
>
