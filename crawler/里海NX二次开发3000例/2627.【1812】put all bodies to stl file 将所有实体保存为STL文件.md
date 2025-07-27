### 【1812】put all bodies to stl file 将所有实体保存为STL文件

#### 代码

```cpp
    /*HEAD PUT_ALL_BODIES_TO_STL_FILE CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_part.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_csys.h>  
    #include <uf_modl.h>  
    #include <uf_cfi.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的内容，UF_print_syslog 是 V18 版本中新增的函数。它用于打印系统日志信息。 */  
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
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
    #define WRITE_ERROR(X) (write_error_to_listing_window(#X, X))  
    void write_error_to_listing_window(char *title, int error)  
    {  
        char  
            err[133],  
            msg[MAX_LINE_SIZE+1];  
        UF_CALL(UF_UI_open_listing_window());  
        UF_get_fail_message(error, err);  
        sprintf(msg, "%s = %d (%s)\n", title, error, err);  
        UF_CALL(UF_UI_write_listing_window(msg));  
    }  
    static int ask_body_faces(tag_t body, tag_p_t *body_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        return make_an_array(&face_list, body_faces);  
    }  
    static void put_bodies_in_stl_file(void *file, int n_bodies, tag_t *bodies)  
    {  
        int  
            body_type,  
            ii,  
            jj,  
            n_errors,  
            n_faces,  
            n_negated;  
        tag_t  
            *faces,  
            *negated,  
            wcs;  
        UF_STD_stl_error_p_t  
            error_info;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        for (ii = 0; ii < n_bodies; ii++)  
        {  
            UF_CALL(UF_MODL_ask_body_type(bodies[ii], &body_type));  
            if (body_type == UF_MODL_SOLID_BODY)  
            {  
                UF_CALL(UF_STD_put_solid_in_stl_file(file, wcs, bodies[ii], 0,  
                    100, 0.02, &n_errors, &error_info));  
            }  
            else  
            {  
                n_faces = ask_body_faces(bodies[ii], &faces);  
                if (n_faces == 1)  
                {  
                    UF_CALL(UF_STD_put_sheets_in_stl_file(file, wcs, 1,  
                        &bodies[ii], 0, 100, 0.02, 0.005, &n_negated, &negated,  
                        &n_errors, &error_info));  
                }  
                else  
                {  
                    for (jj = 0; jj < n_faces; jj++)  
                        UF_CALL(UF_MODL_extract_face(faces[jj], 0, &faces[jj]));  
                    UF_CALL(UF_STD_put_sheets_in_stl_file(file, wcs, n_faces,  
                        faces, 0, 100, 0.02, 0.005, &n_negated, &negated,  
                        &n_errors, &error_info));  
                    UF_CALL(UF_OBJ_delete_array_of_objects(n_faces, faces, NULL));  
                }  
                if (n_negated > 0) UF_free(negated);  
                if (n_faces > 0) UF_free(faces);  
            }  
            if (n_errors > 0) UF_free(error_info);  
        }  
    }  
    static void do_it(void)  
    {  
        int  
            n_bodies;  
        tag_t  
            part = UF_PART_ask_display_part(),  
            *bodies;  
        void  
            *file;  
        char  
            bfn[UF_CFI_MAX_PATH_NAME_SIZE],  
            pfn[UF_CFI_MAX_PATH_NAME_SIZE],  
            tfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        n_bodies = ask_all_bodies(part, &bodies);  
        if (n_bodies > 0)  
        {  
            UF_CALL(UF_PART_ask_part_name(part, pfn));  
            build_similar_filespec(bfn, 78);  
            UF_CALL(UF_STD_open_binary_stl_file(bfn, FALSE, pfn, &file));  
            put_bodies_in_stl_file(file, n_bodies, bodies);  
            UF_CALL(UF_STD_close_stl_file(file));  
            build_similar_filespec(tfn, 79);  
            UF_CALL(UF_STD_open_text_stl_file(tfn, FALSE, &file));  
            put_bodies_in_stl_file(file, n_bodies, bodies);  
            UF_CALL(UF_STD_close_stl_file(file));  
            UF_free(bodies);  
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

> 这段代码是一个NX Open的二次开发示例，主要功能是将NX中的实体模型保存为STL文件。以下是代码的主要功能和流程：
>
> 1. 初始化和错误处理：代码首先初始化NX环境，并定义了一个错误处理函数report_error，用于在函数调用失败时记录错误信息。
> 2. 查询所有实体：通过函数ask_all_bodies查询当前部件中的所有实体，并将它们存储在一个数组中。
> 3. 生成STL文件路径：函数build_similar_filespec用于生成STL文件的路径名。
> 4. 保存实体到STL文件：函数put_bodies_in_stl_file负责将实体数组中的每个实体保存到STL文件中。这里区分了实体和面组两种情况。
> 5. 主函数：函数do_it首先获取当前部件中的所有实体，然后分别保存到二进制和文本格式的STL文件中。
> 6. NX Open入口函数：ufusr是NX Open的入口函数，它调用do_it函数来实现主要功能。
> 7. 卸载函数：ufusr_ask_unload函数用于在卸载时立即关闭文件等资源。
>
> 总体来看，这段代码实现了NX中实体模型到STL文件的转换，为后续的3D打印或其他用途提供了便利。
>
