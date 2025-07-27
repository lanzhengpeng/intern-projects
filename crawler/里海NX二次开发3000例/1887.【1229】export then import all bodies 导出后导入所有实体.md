### 【1229】export then import all bodies 导出后导入所有实体

#### 代码

```cpp
    /*HEAD EXPORT_THEN_IMPORT_ALL_BODIES CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #include <uf_modl.h>  
    #include <uf_csys.h>  
    #define ECHO(X)    (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog 是 V18 新增的函数。在翻译中，请只提供关于 UF_print_syslog 的翻译，无需添加其他无关内容。

UF_print_syslog 在 V18 中是新增的函数。 */  
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
    static tag_t ask_next_body(tag_t part, tag_t body)  
    {  
        int  
            subtype,  
            type;  
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
    static void build_temp_partname(char *copy_fspec)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            *tmp_dir;  
        UF_CALL(UF_translate_variable("UGII_TMP_DIR", &tmp_dir));  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-tmp");  
        UF_CALL(uc4575(tmp_dir, 2, part_name, copy_fspec));  
    }  
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            body = NULL_TAG,  
            *bodies = NULL,  
            grp,  
            part = UF_PART_ask_display_part();  
        uf_list_p_t  
            body_list;  
        char  
            fspec[MAX_FSPEC_SIZE+1];  
        double  
            origin[3],  
            wcs[9];  
        UF_import_part_modes_t  
            modes = { 0, 1, 0, 0, 0, FALSE, 0 };  
        UF_CALL(UF_MODL_create_list(&body_list));  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
            UF_CALL(UF_MODL_put_list_item(body_list, body));  
        n = make_an_array(&body_list, &bodies);  
        build_temp_partname(fspec);  
        UF_CALL(UF_PART_export(fspec, n, bodies));  
        free(bodies);  
        ask_wcs_info(origin, wcs);  
        origin[0] = origin[0] + 10;  
        if (!UF_CALL(UF_PART_import(fspec, &modes, wcs, origin, 1.0, &grp)))  
            ECHO(grp);  
        UF_CALL(uc4561(fspec, 0));  
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

> 这段NX二次开发代码实现了以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误代码和消息。
> 2. 定义了一个函数ask_next_body，用于遍历当前部件中的所有体，并返回下一个体的标签。
> 3. 定义了一个函数allocate_memory，用于分配指定大小的内存。
> 4. 定义了一个函数make_an_array，用于从体列表中创建一个体标签数组，并释放体列表。
> 5. 定义了一个函数build_temp_partname，用于构建临时部件的文件名。
> 6. 定义了一个函数ask_wcs_info，用于获取当前工作坐标系的信息。
> 7. 定义了一个函数do_it，该函数首先遍历当前部件中的所有体，将它们导出到一个临时文件，然后重新导入该文件到部件中。
> 8. 定义了ufusr函数，该函数初始化NX，调用do_it函数执行上述操作，然后终止NX。
> 9. 定义了ufusr_ask_unload函数，用于卸载用户函数。
>
> 总的来说，该代码实现了将当前部件中的所有体导出到一个临时文件，并重新导入该文件的功能，以实现某些特定的操作。
>
