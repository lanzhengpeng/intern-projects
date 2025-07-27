### 【1203】export all solid occurrences to individual parasolid text files 将所有实体出现导出到单独的Parasolid文本文件

#### 代码

```cpp
    /*  
    This example exports the solid occurrences at the displayed part assembly  
    level.  This way the exported data is in the assembly location.  So for the  
    UGII_BASE_DIR\UGOPEN\SampleNXOpenApplications\.NET\AssemblyViewer\toycar_assy.prt  
    it exports 8 bodies (5 wheels, 2 axles, 1 body) to 8 different files.  
    */  
    #include <stdio.h>  
    #include <string.h>  
    #include <stdarg.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_ps.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
        while (! UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &body))  
            && (body != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(body, &type, &subtype));  
            if (subtype == UF_solid_body_subtype) return body;  
        }  
        return NULL_TAG;  
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
    static int ask_all_bodies(tag_t part, tag_t **solids)  
    {  
        tag_t  
            body = NULL_TAG;  
        uf_list_p_t  
            body_list;  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        return (make_an_array(&body_list, solids));  
    }  
    static void build_unique_temp_name(char *fspec, int ftype)  
    {  
        char  
            *tmp_dir,  
            unique[UF_CFI_MAX_FILE_NAME_SIZE];  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_CALL(uc4577(unique));  
        UF_CALL(uc4575(tmp_dir, ftype, unique, fspec));  
    }  
    static uf_list_p_t make_a_list(int count, tag_t *item_array)  
    {  
        int  
            ii;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < count; ii++)  
            UF_CALL(UF_MODL_put_list_item(list, item_array[ii]));  
        return (list);  
    }  
    #define WRITE_D(X) ECHO("%s = %d\n", #X, X);  
    #define WRITE_S(X) (write_string_to_listing_window(#X, X))  
    void write_string_to_listing_window(char *title, char *string)  
    {  
        if (string != NULL)  
            ECHO("%s = \"%s\"\n", title, string);  
        else  
            ECHO("%s = NULL\n", title);  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_bodies;  
        tag_t  
            *bodies,  
            part = UF_PART_ask_display_part();  
        char  
            exported_to[MAX_FSPEC_SIZE+1],  
            *handle;  
        uf_list_p_t  
            body_list;  
        n_bodies = ask_all_bodies(part, &bodies);  
        WRITE_D(n_bodies);  
        for (ii = 0; ii < n_bodies; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(bodies[ii]);  
            handle = UF_TAG_ask_handle_of_tag(bodies[ii]);  
            WRITE_S(handle);  
            UF_free(handle);  
            build_unique_temp_name(exported_to, 0);  
            strcat(exported_to, ".x_t");  
            body_list = make_a_list(1, &bodies[ii]);  
            UF_CALL(UF_PS_export_data(body_list, exported_to));  
            UF_CALL(UF_MODL_delete_list(&body_list));  
            WRITE_S(exported_to);  
        }  
        if (n_bodies > 0) UF_free(bodies);  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发程序，用于导出当前显示的部件装配中的所有实体体。
>
> 主要步骤如下：
>
> 1. 包含必要的NX Open C++ API头文件。
> 2. 定义错误报告函数report_error，用于在出错时打印错误信息。
> 3. 定义ask_next_body函数，用于遍历部件中的所有实体体，返回下一个实体体。
> 4. 定义allocate_memory函数，用于申请内存。
> 5. 定义make_an_array函数，用于将列表转换为数组。
> 6. 定义ask_all_bodies函数，用于获取部件中的所有实体体，并返回数组。
> 7. 定义build_unique_temp_name函数，用于生成唯一的临时文件名。
> 8. 定义make_a_list函数，用于根据数组创建列表。
> 9. 定义write_string_to_listing_window函数，用于在列表窗口打印字符串。
> 10. 定义do_it函数，用于执行导出操作：
>
> 1. 定义ufusr函数，用于初始化NX，执行do_it函数，然后终止NX。
> 2. 定义ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，这段代码实现了NX部件中所有实体体的导出功能，每个实体体会被导出到一个单独的文件中。
>
