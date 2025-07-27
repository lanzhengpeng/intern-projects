### 【0964】create udo linked to datum axis 创建与基准轴关联的UDO

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_udobj.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_exit.h>  
    #include <uf_vec.h>  
    #include <uf_modl.h>  
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
    static UF_UDOBJ_class_t MY_class_id = 0;  
    DllExport extern UF_UDOBJ_class_t get_my_class_id(void)  
    {  
        return MY_class_id;  
    }  
    /* qq3123197280 */  
    DllExport extern void update_func(tag_t udo, UF_UDOBJ_link_p_t update_cause)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        uc1601("Linked object has been modified", 1);  
        UF_terminate();  
    }  
    static void register_udo_class()  
    {  
        UF_CALL(UF_UDOBJ_create_class("MY_test", "my UDOs", &MY_class_id));  
        UF_CALL(UF_UI_add_to_class_sel(MY_class_id));  
        UF_CALL(UF_UDOBJ_register_update_cb(MY_class_id, update_func));  
    }  
    /* qq3123197280 */  
    void ufsta(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        register_udo_class();  
        UF_terminate();  
    }  
    static logical specify_vector(char *prompt, double vec[3], double point[3])  
    {  
        int  
            mode = UF_UI_NO_DEFAULT,  
            resp;  
        UF_CALL(UF_UI_specify_vector(prompt, &mode, UF_UI_DISP_TEMP_VECTOR,  
            vec, point, &resp));  
        if (resp == UF_UI_OK) return TRUE;  
        else return FALSE;  
    }  
    static int allocate_memory(unsigned int nbytes, void **where)  
    {  
        int  
            resp;  
        *where = UF_allocate_memory(nbytes, &resp);  
        return resp;  
    }  
    static int ask_udo_links(tag_t udo, UF_UDOBJ_link_t **links)  
    {  
        int  
            ii,  
            n;  
        UF_UDOBJ_all_data_t  
            data;  
        UF_CALL(UF_UDOBJ_ask_udo_data(udo, &data));  
        n = data.num_links;  
        if (!UF_CALL(allocate_memory(n * sizeof(UF_UDOBJ_link_t), (void **)links)))  
            for (ii = 0; ii < n; ii++)  
                (*links)[ii] = data.link_defs[ii];  
        UF_CALL(UF_UDOBJ_free_udo_data(&data));  
        return n;  
    }  
    static update_model_avoid_udo_callbacks(tag_t udo)  
    {  
        int  
            ii,  
            n_links;  
        UF_UDOBJ_link_t  
            *links;  
        n_links = ask_udo_links(udo, &links);  
        for (ii = 0; ii < n_links; ii++)  
            UF_CALL(UF_UDOBJ_delete_link(udo, &links[ii]));  
        UF_CALL(UF_MODL_update());  
        if (n_links > 0)  
        {  
            UF_CALL(UF_UDOBJ_add_links(udo, n_links, links));  
            UF_free(links);  
        }  
    }  
    static void do_it(void)  
    {  
        tag_t  
            datum_axis,  
            datum_feat,  
            udo;  
        double  
            move_data[2][12] = { 0,0,0, 0,0,0,0,0,0,0,0,0,  
                                 1,0,0, 0,0,0,0,0,0,0,0,0 },  
            point1[3],  
            point2[3],  
            vec[3];  
        uf_list_p_t  
            feat_list;  
        UF_UDOBJ_link_t  
            link_def = { 3, NULL_TAG, FALSE };  
        UF_UDOBJ_class_t  
            udo_class_id = get_my_class_id();  
        if (udo_class_id == 0)  
        {  
            register_udo_class();  
            udo_class_id = get_my_class_id();  
        }  
        if (specify_vector("Datum axis", vec, point1))  
        {  
            UF_VEC3_add(point1, vec, point2);  
            UF_CALL(UF_MODL_create_fixed_daxis(point1, point2, &datum_axis));  
            UF_CALL(UF_UDOBJ_create_udo(udo_class_id, &udo));  
            link_def.assoc_ug_tag = datum_axis;  
            UF_CALL(UF_UDOBJ_add_links(udo, 1, &link_def));  
            UF_CALL(UF_MODL_create_list(&feat_list));  
            UF_CALL(UF_MODL_ask_object_feat(datum_axis, &datum_feat));  
            UF_CALL(UF_MODL_put_list_item(feat_list, datum_feat));  
            uc1601("Moving datum axis linked to UDO...", TRUE);  
            UF_CALL(UF_MODL_move_feature(feat_list, 0, move_data));  
            UF_CALL(UF_MODL_update());  
            uc1601("Now to avoid UDO callback...", TRUE);  
            UF_CALL(UF_MODL_move_feature(feat_list, 0, move_data));  
            update_model_avoid_udo_callbacks(udo);  
            UF_CALL(UF_MODL_delete_list(&feat_list));  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (UF_CALL(UF_initialize())) return;  
        do_it();  
        UF_terminate();  
    }

```

#### 代码解析

> 这段代码是用于NX的二次开发示例，主要功能是创建用户定义对象（UDO）来管理一个基准轴，并在移动基准轴时更新UDO。代码的主要逻辑如下：
>
> 1. 定义了一个名为“MY_test”的UDO类，并注册了更新回调函数update_func，用于在UDO关联的对象被修改时执行。
> 2. ufsta函数用于在启动NX时注册UDO类。
> 3. do_it函数是主要业务逻辑，首先提示用户指定一个基准轴向量，然后创建基准轴和对应的UDO对象，并将基准轴关联到UDO。
> 4. ufusr函数是NX的UF运行函数，调用do_it来执行业务逻辑。
> 5. 在移动基准轴时，UDO的更新回调函数会被调用。为了避免重复调用，在移动前先删除UDO的所有关联，移动后再重新添加。
> 6. 使用了UF_MODL模块来创建和管理基准轴、特征列表等。
> 7. 使用了UF_UDOBJ模块来创建和管理UDO对象以及UDO关联。
> 8. 使用了UF_UI模块来获取用户输入。
> 9. 使用了UF_VEC模块进行向量运算。
> 10. 使用了UF_ALLOCATE_MEMORY来分配内存。
> 11. 使用了错误处理宏UF_CALL来捕获函数调用错误。
>
> 总的来说，这段代码展示了如何在NX中通过UDO来管理模型元素，并处理UDO更新回调，具有很好的NX二次开发参考价值。
>
