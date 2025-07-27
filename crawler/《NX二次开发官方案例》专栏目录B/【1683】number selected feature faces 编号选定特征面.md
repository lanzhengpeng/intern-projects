### 【1683】number selected feature faces 编号选定特征面

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
    static void number_objects(tag_t *objects, int n_objects)  
    {  
        logical  
            is_displayable;  
        int  
            ii;  
        double  
            loc[3];  
        char  
            msg[133];  
        UF_DISP_refresh();  
        for (ii = 0; ii < n_objects; ii++)  
        {  
            UF_CALL(UF_OBJ_is_displayable(objects[ii], &is_displayable));  
            if (!is_displayable) continue;  
            sprintf(msg, "%d", ii+1);  
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
        while ((feature = select_a_feature("Number faces")) != NULL_TAG)  
        {  
            n_faces = ask_feature_faces(feature, &faces);  
            number_objects(faces, n_faces);  
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

> 这段代码是一个NX Open C++的二次开发示例，主要功能如下：
>
> 1. 定义了一个错误报告函数report_error，用于在出错时输出错误码和错误信息。
> 2. 定义了一个选择特征的函数select_a_feature，用于提示用户选择一个特征，并返回该特征的对象标签。
> 3. 定义了一个显示临时文字的函数display_temporary_text，用于在指定的位置显示一段文字。
> 4. 定义了一个为对象编号的函数number_objects，用于遍历对象数组，并在每个对象的原点位置显示其序号。
> 5. 定义了一个内存分配函数allocate_memory，用于分配指定大小的内存。
> 6. 定义了一个从列表创建数组的函数make_an_array，用于从UF_MODL列表创建一个tag数组。
> 7. 定义了一个获取特征面的函数ask_feature_faces，用于获取特征的所有面，并返回一个tag数组。
> 8. 定义了一个主函数do_it，用于循环提示用户选择特征，为每个特征的面编号。
> 9. 定义了ufusr函数，作为二次开发的入口函数，在NX启动时调用，主要完成初始化、执行do_it函数，以及终止时的清理工作。
> 10. 定义了卸载函数ufusr_ask_unload，用于在卸载二次开发模块时调用。
>
> 总体来说，这段代码实现了一个简单的交互式NX二次开发示例，通过选择特征，为特征的面编号，并在图形区显示，从而演示了NX二次开发的基本流程。
>
