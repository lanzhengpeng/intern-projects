### 【2223】report new expressions after importing specified part 报告导入指定部件后新增的表达式

#### 代码

```cpp
    /*HEAD REPORT_NEW_EXPRESSIONS_AFTER_IMPORTING_SPECIFIED_PART CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <uf_part.h>  
    #include <uf_assem.h>  
    #include <uf_csys.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是V18版本中新增的函数。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_D(X) (write_integer_to_listing_window(#X, X))  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static void combine_directory_and_wildcard(char *dir, char *fltr, char *spec)  
    {  
        if (!strcmp(dir, ""))  
            strcpy(spec, fltr);  
        else  
        {  
            UF_CALL(uc4575(dir, 0, "junk.xxx", spec));  
            strcpy(strstr(spec, "junk.xxx"), fltr);  
        }  
    }  
    static logical prompt_for_part_name(char *prompt, char *fspec)  
    {  
        int  
            resp;  
        char  
            filter[MAX_FSPEC_SIZE+1],  
            *p_dir,  
            *p_ext;  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_PART_DIR, &p_dir));  
        UF_CALL(UF_UI_ask_dialog_filter(UF_UI_PART_OPEN_FLTR, &p_ext));  
        combine_directory_and_wildcard(p_dir, p_ext, filter);  
        UF_free(p_dir);  
        UF_free(p_ext);  
        if (!UF_CALL(UF_UI_create_filebox(prompt, "Part Name", filter, "", fspec,  
            &resp)) && (resp != UF_UI_CANCEL)) return TRUE;  
        else return FALSE;  
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
    static int ask_whats_new(tag_t *old, int n_old, tag_t *now, int n_now,  
        tag_t **new)  
    {  
        int  
            ii,  
            jj;  
        uf_list_p_t  
            list;  
        UF_CALL(UF_MODL_create_list(&list));  
        for (ii = 0; ii < n_now; ii++)  
        {  
            for (jj = 0; jj < n_old; jj++)  
                if (old[jj] == now[ii]) break;  
            if (jj == n_old) UF_CALL(UF_MODL_put_list_item(list, now[ii]));  
        }  
        return (make_an_array(&list, new));  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_after,  
            n_before,  
            n_new;  
        tag_t  
            *after,  
            *before,  
            disp_part = UF_PART_ask_display_part(),  
            grp,  
            *new,  
            work_part = UF_ASSEM_ask_work_part();  
        char  
            fspec[MAX_FSPEC_SIZE+1] = { "" },  
            *string;  
        double  
            dest_csys[9],  
            dest_pnt[3];  
        UF_import_part_modes_t  
            modes = { 0, 1, 0, 0, 0, FALSE, 0 };  
        if (prompt_for_part_name("Import Part", fspec))  
        {  
            UF_CALL(UF_MODL_ask_exps_of_part(work_part, &n_before, &before));  
            ask_wcs_info(dest_pnt, dest_csys);  
            if (work_part != disp_part)  
                UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, dest_pnt,  
                UF_CSYS_WORK_COORDS, dest_pnt));  
            UF_CALL(UF_PART_import(fspec, &modes, dest_csys, dest_pnt, 1.0, &grp));  
            UF_CALL(UF_MODL_ask_exps_of_part(work_part, &n_after, &after));  
            n_new = ask_whats_new(before, n_before, after, n_after, &new);  
            WRITE_D(n_new);  
            for (ii = 0; ii < n_new; ii++)  
            {  
                UF_CALL(UF_MODL_ask_exp_tag_string(new[ii], &string));  
                WRITE(string);  
                WRITE("\n");  
                UF_free(string);  
            }  
            if (n_before > 0) UF_free(before);  
            if (n_after > 0) UF_free(after);  
            if (n_new > 0) UF_free(new);  
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

> 这是段NX二次开发代码，主要功能是导入一个指定部件，并报告导入后新增的表达式。具体步骤如下：
>
> 1. 包含必要的NX API头文件。
> 2. 定义UF_CALL宏，用于调用NX API函数，并捕获错误。
> 3. 定义report_error函数，用于报告错误。
> 4. 定义WRITE和WRITE_D宏，用于在列表窗口输出信息。
> 5. 定义write_integer_to_listing_window函数，用于在列表窗口输出整数值。
> 6. 定义combine_directory_and_wildcard函数，用于组合目录和通配符。
> 7. 定义prompt_for_part_name函数，用于提示用户输入部件文件路径。
> 8. 定义ask_wcs_info函数，用于获取当前坐标系信息。
> 9. 定义allocate_memory函数，用于分配内存。
> 10. 定义make_an_array函数，用于从列表中提取对象数组。
> 11. 定义ask_whats_new函数，用于找出新增的对象。
> 12. 定义do_it函数，用于执行导入部件和报告新增表达式的逻辑。
> 13. 定义ufusr函数，作为二次开发的入口函数，调用do_it。
> 14. 定义ufusr_ask_unload函数，返回卸载类型。
>
> 总体来说，该代码通过NX API实现了部件的导入和新增表达式的报告，为二次开发提供了示例。
>
