### 【1832】recreate all sketch dimensional constraints 重新创建所有草图尺寸约束

#### 代码

```cpp
    #include <stdio.h>  
    #include <string.h>  
    #include <stdlib.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_sket.h>  
    #include <uf_part.h>  
    #include <uf_csys.h>  
    #include <uf_mtx.h>  
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
    static tag_t ask_next_new_sketch(tag_t part, tag_t sketch)  
    {  
        int  
            subtype,  
            type;  
        while  
            (!UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_sketch_type, &sketch)) &&  
            (sketch != NULL_TAG))  
        {  
            UF_CALL(UF_OBJ_ask_type_and_subtype(sketch, &type, &subtype));  
            if (subtype == UF_v13_sketch_subtype) return (sketch);  
        }  
        return NULL_TAG;  
    }  
    static tag_t set_wcs_to_sketch(tag_t sketch)  
    {  
        double  
            csys[9];  
        tag_t  
            mx,  
            orig_wcs,  
            wcs;  
        UF_SKET_info_t  
            sket_info;  
        UF_CALL(UF_CSYS_ask_wcs(&orig_wcs));  
        UF_CALL(UF_SKET_ask_sketch_info(sketch, &sket_info));  
        UF_CALL(UF_MTX3_initialize(sket_info.csys, &sket_info.csys[3], csys));  
        UF_CALL(UF_CSYS_create_matrix(csys, &mx));  
        UF_CALL(UF_CSYS_create_temp_csys(&sket_info.csys[9], mx, &wcs));  
        UF_CALL(UF_CSYS_set_wcs(wcs));  
        return orig_wcs;  
    }  
    static void do_it(void)  
    {  
        int  
            ii,  
            n_dims;  
        tag_t  
            *dims,  
            exp,  
            junk,  
            orig_wcs,  
            part = UF_PART_ask_display_part(),  
            sketch = NULL_TAG;  
        double  
            origin[3];  
        char  
            sket_name[UF_OBJ_NAME_LEN];  
        UF_SKET_con_type_t  
            dim_type;  
        UF_SKET_dim_object_t  
            object[2];  
        while ((sketch = ask_next_new_sketch(part, sketch)) != NULL_TAG)  
        {  
            UF_CALL(UF_OBJ_ask_name(sketch, sket_name));  
            UF_CALL(UF_SKET_initialize_sketch(sket_name, &junk));  
            uc1601("Before", TRUE);  
        //  Begin work around PR 6547295  
            orig_wcs = set_wcs_to_sketch(sketch);  
        //  End work around PR 6547295  
            UF_CALL(UF_SKET_ask_dimensions_of_sketch(sketch, &n_dims, &dims));  
            for (ii = 0; ii < n_dims; ii++)  
            {  
                UF_CALL(UF_SKET_read_dimension(sketch, dims[ii], &dim_type,  
                    &object[0], &object[1], origin, &exp));  
                UF_CALL(UF_SKET_delete_dimensions(1, &dims[ii]));  
                UF_CALL(UF_SKET_create_dimensional_constraint(sketch, dim_type,  
                    2, object, origin, &junk));  
            }  
            UF_free(dims);  
            UF_CALL(UF_DISP_regenerate_display());  
            uc1601("After", TRUE);  
            UF_CALL(UF_CSYS_set_wcs(orig_wcs));  // work around PR 6547295  
            UF_CALL(UF_SKET_terminate_sketch());  
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

> 这段代码是一个NX二次开发示例，其主要功能是遍历零件中的草图，将草图约束转换为尺寸约束。以下是代码的主要功能：
>
> 1. 错误报告：定义了ECHO和report_error函数，用于打印错误信息和调用UF函数时的错误处理。
> 2. 草图遍历：ask_next_new_sketch函数用于遍历零件中的草图，并返回下一个NX 13版本的草图。
> 3. 工作坐标系设置：set_wcs_to_sketch函数用于将工作坐标系设置为草图坐标系，以解决NX的一个bug。
> 4. 草图约束处理：do_it函数用于遍历每个草图，获取草图中的所有尺寸约束，删除这些尺寸约束，然后重新创建为尺寸约束。
> 5. NX初始化和终止：ufusr函数用于初始化和终止NX Open，并调用do_it函数。
> 6. 卸载请求：ufusr_ask_unload函数用于在卸载NX Open时立即卸载。
>
> 总的来说，这段代码通过遍历零件中的草图，将草图约束转换为尺寸约束，以实现对草图的有效管理。
>
