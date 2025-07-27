### 【2909】specify max facet edge length for stl output file 指定STL输出文件的最大面边长度

#### 代码

```cpp
    /* SPECIFY_MAX_FACET_EDGE_LENGTH_FOR_STL_OUTPUT_FILE CCC UFUN 里海译:对于STL输出文件，请指定最大面边长度。 */  
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
    #define ECHO(X) { UF_UI_open_listing_window(); \  
        UF_UI_write_listing_window(X); \  
        UF_print_syslog(X, FALSE); }  
    #define UF_CALL(X) (report_error( __FILE__, __LINE__, #X, (X)))  
    static int report_error( char *file, int line, char *call, int irc)  
    {  
        if (irc)  
        {  
            char err[133],  
                 msg[UF_UI_MAX_STRING_LEN];  
            sprintf_s( msg, sizeof( msg ),   
                       "*** ERROR code %d at line %d in %s:\n+++ ",  
                                                   irc, line, file );  
            UF_get_fail_message(irc, err);  
            ECHO(msg);  
            ECHO(err);  
            ECHO("\n");  
            ECHO(call);  
            ECHO(";\n");  
        }  
        return(irc);  
    }  
    #define WRITE(X)   UF_UI_open_listing_window(); UF_UI_write_listing_window(X)  
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
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            msg[UF_CFI_MAX_PATH_NAME_SIZE + 100];  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        UF_CALL(uc4575(dspec, ftype, part_name, fspec));  
        sprintf_s( msg, sizeof( msg ), "The output file is: %s\n", fspec );  
        WRITE( msg );  
    }  
    static int ask_body_faces(tag_t body, tag_p_t *body_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        return make_an_array(&face_list, body_faces);  
    }  
    static logical prompt_for_a_number(char *prompt, char *item, double *number)  
    {  
        int  
            irc,  
            resp;  
        char  
            menu[1][16];  
        double  
            da[1];  
        strncpy(&menu[0][0], item, 16);  
        menu[0][15] = '\0';  
        da[0] = *number;  
        resp = uc1609(prompt, menu, 1, da, &irc);  
        if (resp == 3 || resp == 4)  
        {  
            *number = da[0];  
            return TRUE;  
        }  
        else return FALSE;  
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
        double  
            max_edge_length = 0.0;  
        UF_STD_stl_error_p_t  
            error_info;  
        logical  
            number = FALSE;  
        number = prompt_for_a_number("Max Edge Length",   
                                     "Max Edge Length",&max_edge_length );  
        if (number == FALSE ) return;  
        UF_CALL(UF_CSYS_ask_wcs(&wcs));  
        for (ii = 0; ii < n_bodies; ii++)  
        {  
            UF_CALL(UF_MODL_ask_body_type(bodies[ii], &body_type));  
            if (body_type == UF_MODL_SOLID_BODY)  
            {  
                UF_CALL(UF_STD_put_solid_in_stl_file(file, wcs, bodies[ii], 0,  
                    max_edge_length, 0.02, &n_errors, &error_info));  
            }  
            else  
            {  
                n_faces = ask_body_faces(bodies[ii], &faces);  
                if (n_faces == 1)  
                {  
                    UF_CALL(UF_STD_put_sheets_in_stl_file(file, wcs, 1,  
                                       &bodies[ii], 0, max_edge_length,   
                                     0.02, 0.005, &n_negated, &negated,  
                                                 &n_errors, &error_info));  
                }  
                else  
                {  
                    for (jj = 0; jj < n_faces; jj++)  
                        UF_CALL(UF_MODL_extract_face(faces[jj], 0, &faces[jj]));  
                    UF_CALL(UF_STD_put_sheets_in_stl_file(file, wcs, n_faces,  
                                                   faces, 0, max_edge_length,   
                                           0.02, 0.005, &n_negated, &negated,  
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
            pfn[UF_CFI_MAX_PATH_NAME_SIZE];  
        n_bodies = ask_all_bodies(part, &bodies);  
        if (n_bodies > 0)  
        {  
            UF_CALL(UF_PART_ask_part_name(part, pfn));  
            build_similar_filespec(bfn, 78);  
            UF_CALL(UF_STD_open_binary_stl_file(bfn, FALSE, pfn, &file));  
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

> 这段代码是一个NX的二次开发示例，其主要功能是将NX中的实体输出为STL文件。下面是代码的主要流程：
>
> 1. 初始化：使用UF_initialize进行初始化，并调用do_it函数执行主体流程。
> 2. 获取当前显示的部件：通过UF_PART_ask_display_part获取当前显示的部件。
> 3. 获取部件中的所有实体：通过ask_all_bodies函数循环获取部件中的所有实体，并存储在一个tag_t数组中。
> 4. 构建STL文件路径：通过build_similar_filespec函数构建STL文件的路径，格式为“部件名.stl”。
> 5. 打开STL文件：通过UF_STD_open_binary_stl_file函数打开构建的STL文件路径。
> 6. 遍历实体数组：遍历实体数组，对每个实体调用put_bodies_in_stl_file函数进行输出。
> 7. 输出实体到STL文件：在put_bodies_in_stl_file函数中，根据实体的类型，调用不同的函数输出为STL文件。
> 8. 关闭STL文件：最后，关闭STL文件。
> 9. 清理资源：释放实体数组和STL文件句柄。
> 10. 反初始化：最后，调用UF_terminate进行反初始化。
>
> 总的来说，这段代码实现了NX中实体的STL格式输出，其中主要难点包括遍历部件中的实体、构建STL文件路径以及根据实体类型进行不同的STL输出处理。
>
