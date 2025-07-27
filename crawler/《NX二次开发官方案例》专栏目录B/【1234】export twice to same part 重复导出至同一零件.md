### 【1234】export twice to same part 重复导出至同一零件

#### 代码

```cpp
    /*HEAD EXPORT_TWICE_TO_SAME_PART CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_disp.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:翻译：UF_print_syslog在V18中新增。 */  
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
    static int select_objects(char *prompt, tag_t **objects)  
    {  
        int  
            cnt,  
            ii,  
            resp;  
        tag_t  
            *objs;  
        UF_CALL(UF_UI_select_with_class_dialog( prompt, "",  
            UF_UI_SEL_SCOPE_WORK_PART, NULL, NULL, &resp, &cnt, objects));  
        objs = *objects;  
        for (ii = 0; ii < cnt; ii++)  
            UF_CALL(UF_DISP_set_highlight(objs[ii], 0));  
        return cnt;  
    }  
    static void set_wcs(double origin[3], double dest_csys[9])  
    {  
        tag_t  
            csys,  
            mx;  
        UF_MTX3_ortho_normalize(dest_csys);  
        UF_CALL(UF_CSYS_create_matrix(dest_csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(origin, mx, &csys));  
        UF_CALL(UF_CSYS_set_wcs(csys));  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            *objs,  
            wcs;  
        double  
            zero[3] = { 0,0,0 },  
            abs[9] = { 1,0,0, 0,1,0, 0,0,1 };  
        UF_PART_export_options_t  
            options = { TRUE, UF_PART_remove_params, UF_PART_copy_exp_shallowly };  
    /*  The WCS must be at absolute - see PR 730611 里海译:根据 PR 730611，WCS 必须是绝对的。 */  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        set_wcs(zero, abs);  
        if ((n = select_objects("Select objects for 1st export", &objs)) > 0)  
        {  
            UF_CALL(UF_PART_export_with_options("/tmp/temp", n, objs, &options));  
            free(objs);  
            options.new_part = FALSE;  
        }  
        if ((n = select_objects("Select objects for 2nd export", &objs)) > 0)  
        {  
            UF_CALL(UF_PART_export_with_options("/tmp/temp", n, objs, &options));  
            free(objs);  
        }  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
    }  
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

> 这段NX二次开发代码主要实现以下功能：
>
> 1. 定义了一个错误报告函数report_error，用于在UF函数调用出错时打印错误信息。
> 2. 定义了一个选择对象的函数select_objects，用于通过对话框让用户选择要导出的对象，并返回对象数组。
> 3. 定义了一个设置WCS的函数set_wcs，用于设置绝对坐标系为当前坐标系。
> 4. 主函数do_it实现了以下逻辑：获取当前WCS，并保存为wcs。设置绝对坐标系为当前坐标系。让用户选择对象，并调用UF_PART_export_with_options函数导出这些对象到/tmp/temp文件。第一次导出时，会删除参数并浅拷贝实体。第二次导出时，不会创建新部件。恢复wcs。
> 5. 获取当前WCS，并保存为wcs。
> 6. 设置绝对坐标系为当前坐标系。
> 7. 让用户选择对象，并调用UF_PART_export_with_options函数导出这些对象到/tmp/temp文件。第一次导出时，会删除参数并浅拷贝实体。第二次导出时，不会创建新部件。
> 8. 恢复wcs。
> 9. ufusr是用户自定义函数，在NX启动时会被调用。它初始化UF，调用do_it函数，然后终止UF。
> 10. ufusr_ask_unload函数返回立即卸载标志，表示NX可以立即卸载这个UF文件。
>
> 综上，这段代码的主要功能是让用户选择对象，并支持将同一组对象导出到同一文件两次，第一次删除参数并浅拷贝实体，第二次不创建新部件。同时，通过错误报告和坐标系管理，保证了代码的健壮性。
>
