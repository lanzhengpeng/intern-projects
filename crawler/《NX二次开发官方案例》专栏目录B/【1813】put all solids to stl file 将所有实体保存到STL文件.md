### 【1813】put all solids to stl file 将所有实体保存到STL文件

#### 代码

```cpp
    /*HEAD PUT_ALL_SOLIDS_TO_STL_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_std.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，UF_print_syslog是在V18版本中新增的功能。 */  
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
    static void build_similar_filespec(char *fspec, int ftype)  
    {  
        tag_t  
            part = UF_PART_ask_display_part();  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
    }  
    static void do_it(void)  
    {  
        int  
            body_type,  
            ii,  
            num_errors;  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part(),  
            wcs;  
        void  
            *file;  
        char  
            ofn[UF_CFI_MAX_PATH_NAME_SIZE],  
            pfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        UF_STD_stl_error_p_t  
            error_info;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        UF_PART_ask_part_name(part, pfn);  
        build_similar_filespec(ofn, 78);  
        for (body = ask_next_body(part, body);  
             body != NULL_TAG;  
             body = ask_next_body(part, body))  
        {  
            UF_CALL(UF_MODL_ask_body_type(body, &body_type));  
            if (body_type == UF_MODL_SOLID_BODY)  
            {  
                UF_CALL(UF_STD_open_binary_stl_file(ofn, TRUE, pfn, &file));  
                UF_CALL(UF_STD_put_solid_in_stl_file(file, wcs, body, 0, 0, 0.02,  
                    &num_errors, &error_info));  
                for (ii = 0; ii < num_errors; ii++)  
                    UF_CALL(error_info[ii].error_code);  
                if (num_errors > 0) UF_free(error_info);  
                UF_CALL(UF_STD_close_stl_file(file));  
            }  
        }  
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

> 这段代码是NX二次开发的一个UF函数，用于将当前部件中的所有实体实体写入到一个STL文件中。以下是代码的详细解释：
>
> 1. 头文件：引入了必要的NX Open C API头文件，用于进行NX的二次开发。
> 2. 错误处理函数：定义了一个错误处理函数report_error，用于打印函数调用错误信息。
> 3. 获取下一个实体函数：定义了一个函数ask_next_body，用于遍历当前部件中的实体，并返回下一个实体。
> 4. 构建相似文件名函数：定义了一个函数build_similar_filespec，用于根据当前部件名构建STL文件的相似文件名。
> 5. 主逻辑函数：定义了一个函数do_it，用于遍历当前部件中的所有实体，并将每个实体写入到STL文件中。在写入时，会判断实体的类型，只写入实体实体。
> 6. UF函数：定义了UF函数ufusr，用于初始化NX并调用主逻辑函数。
> 7. 卸载函数：定义了一个卸载函数ufusr_ask_unload，用于在卸载该UF函数时立即卸载。
>
> 整体而言，这段代码实现了将NX部件中的实体实体写入STL文件的功能，采用了典型的NX二次开发流程，并具有良好的错误处理机制。
>
