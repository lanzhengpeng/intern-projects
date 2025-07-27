### 【1228】export then import all bodies 导出后导入所有体

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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的函数，因此翻译为：注意：UF_print_syslog是在V18版本中新增的函数。 */  
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

> 这段代码是一个NX Open C++ API编写的NX二次开发示例，主要实现以下功能：
>
> 1. 定义了一些宏和函数来报告错误，包括ECHO和UF_CALL宏，以及report_error函数。
> 2. 实现了ask_next_body函数，用于遍历零件中的实体，并返回下一个实体的tag。
> 3. 实现了allocate_memory和make_an_array函数，用于分配内存并创建tag数组。
> 4. 实现了build_temp_partname函数，用于构建临时零件文件的路径。
> 5. 实现了ask_wcs_info函数，用于获取当前工作坐标系的信息。
> 6. 实现了do_it函数，用于导出当前显示的零件中的所有实体，然后导入到一个临时文件，并修改坐标系后再次导入。
> 7. 实现了ufusr和ufusr_ask_unload函数，分别用于初始化NX、执行do_it函数，以及卸载用户函数。
>
> 总体来说，该代码通过NX Open C++ API实现了NX二次开发的功能，包括遍历零件、导出导入、坐标系操作等，是一个比较完整的NX二次开发示例。
>
