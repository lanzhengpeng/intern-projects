### 【3121】import part as group and report group info 导入零件作为组，并报告组信息

#### 代码

```cpp
    /*HEAD IMPORT_PART_AS_GROUP_AND_REPORT_GROUP_INFO CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_assem.h>  
    #include <uf_group.h>  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    /* qq3123197280 */  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[133];  
            sprintf(msg, "*** ERROR code %d at line %d in %s:\n+++ ",  
                irc, line, file);  
            UF_get_fail_message(irc, err);  
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档，UF_print_syslog 是 V18 版本中新增的功能。文档只要求提供翻译，而不需要其他内容。 */  
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
    #define WRITENZ(X) if (X != 0) (write_integer_to_listing_window(#X, X))  
    /* qq3123197280 */  
    static void write_integer_to_listing_window(char *title, int n)  
    {  
        char  
            msg[UF_UI_MAX_STRING_LEN+1];  
        UF_CALL(UF_UI_open_listing_window());  
        sprintf(msg, "%s = %d\n", title, n);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    /* qq3123197280 */  
    static void ask_wcs_info(double origin[3], double axes[9])  
    {  
        tag_t  
            wcs,  
            wcs_mx;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_CALL(UF_CSYS_ask_csys_info(wcs, &wcs_mx, origin));  
        UF_CALL(UF_CSYS_ask_matrix_values(wcs_mx, axes));  
    }  
    /* qq3123197280 */  
    static void report_group_members(tag_t object)  
    {  
        unsigned int  
            sub_file_id,  
            version;  
        int  
            ii,  
            n;  
        char  
            *file_data,  
            *handle,  
            msg[MAX_LINE_SIZE+1],  
            obj_name[MAX_ENTITY_NAME_SIZE + 1];  
        tag_t  
            *members;  
        sprintf( msg, "group %d", object);  
        WRITE( msg );  
        if (!UF_OBJ_ask_name(object, obj_name))  
        {  
            sprintf( msg, ", named %s, ", obj_name);  
            WRITE( msg );  
        }  
        UF_CALL(UF_GROUP_ask_group_data(object, &members, &n));  
        sprintf( msg, " has %d group members\n", n );  
        for (ii = 0; ii < n; ii++)  
        {  
            WRITE_D(ii);  
            WRITE_D(members[ii]);  
            handle = UF_TAG_ask_handle_of_tag(members[ii]);  
            UF_CALL(UF_TAG_decompose_handle(handle, &file_data, &sub_file_id,  
                &version));  
            WRITE_D(sub_file_id);  
            UF_free(handle);  
            UF_free(file_data);  
        }  
        if (n > 0) UF_free(members);  
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
    static void do_it(void)  
    {  
        int  
            resp;  
        tag_t  
            grp = NULL_TAG,  
            disp_part = UF_PART_ask_display_part(),  
            work_part = UF_ASSEM_ask_work_part();  
        char  
            *dir_name,  
            filter[MAX_FSPEC_SIZE+1],  
            fspec[MAX_FSPEC_SIZE+1] = { "" };  
        double  
            dest_csys[9],  
            dest_pnt[3];  
        UF_import_part_modes_t  
            modes = { 0, 1, 0, 0, 0, FALSE, 0 };  
        ask_wcs_info(dest_pnt, dest_csys);  
        if (work_part != disp_part)  
            UF_CALL(UF_CSYS_map_point(UF_CSYS_ROOT_COORDS, dest_pnt,  
            UF_CSYS_WORK_COORDS, dest_pnt));  
        UF_CALL(UF_UI_ask_dialog_directory(UF_UI_IMPORT_DIR, &dir_name));  
        if (!strcmp(dir_name, ""))  
        {  
            uc4565(1, fspec);  
            UF_CALL(UF_UI_set_dialog_directory(UF_UI_IMPORT_DIR, fspec));  
        }  
        else  
            strcpy(fspec, dir_name);  
        UF_free(dir_name);  
        combine_directory_and_wildcard(fspec, "*.prt", filter);  
        if (!UF_UI_create_filebox("Choose part to import", "Import Part",  
            filter, "", fspec, &resp) && (resp != UF_UI_CANCEL))  
        {  
            UF_CALL(UF_PART_import(fspec, &modes, dest_csys, dest_pnt, 1.0, &grp));  
            if( NULL_TAG != grp ) report_group_members( grp );  
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
> 1. 定义了一个错误报告函数report_error，用于打印UF函数调用失败的错误信息。
> 2. 定义了一个写入整数到列表窗口的函数write_integer_to_listing_window，用于打印变量名和值。
> 3. 定义了一个获取当前工作坐标系信息的函数ask_wcs_info。
> 4. 定义了一个报告组内成员信息的函数report_group_members。
> 5. 定义了一个组合目录名和通配符的函数combine_directory_and_wildcard。
> 6. 定义了主函数do_it，用于导入一个prt文件，并打印导入的组件组内的成员信息。
> 7. 定义了ufusr函数，作为程序的入口，调用do_it函数。
> 8. 定义了ufusr_ask_unload函数，返回立即卸载标志。
>
> 整体上，这段代码通过UF函数实现了导入组件、获取组件组信息、打印信息等功能，是一个简单的NX二次开发示例。
>
