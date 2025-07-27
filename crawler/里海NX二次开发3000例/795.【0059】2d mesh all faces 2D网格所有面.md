### 【0059】2d mesh all faces 2D网格所有面

#### 代码

```cpp
    /*HEAD 2D_MESH_ALL_FACES CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_obj.h>  
    #include <uf_modl.h>  
    #include <uf_sf.h>  
    #include <uf_disp.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是V18版本中新增的函数。文档要求只回答译文，不要添加其他无关内容。 */  
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
    static int ask_body_faces(tag_t body, tag_p_t *body_faces)  
    {  
        uf_list_p_t  
            face_list;  
        UF_CALL(UF_MODL_ask_body_faces(body, &face_list));  
        return make_an_array(&face_list, body_faces);  
    }  
    static void do_it(void)  
    {  
        int  
            n;  
        tag_t  
            body = NULL_TAG,  
            *faces,  
            mesh_id,  
            part = UF_PART_ask_display_part(),  
            scenario;  
        UF_SF_sfqm_mesh_data_t  
            mesh_data;  
        UF_SF_sfqm_param_data_t  
            parm_data = { 0.02, 0.0001, 0.01, 0.001, 3 };  
        mesh_data.elem_type = UF_SF_SFQM_TRIA3;  
        mesh_data.elem_size = 3;  
        mesh_data.mesh_param_data_p = &parm_data;  
    /*  New members in V16.0  
        mesh_data.smart_edge_seeding = FALSE;  
        mesh_data.do_not_format_mesh = FALSE;  
    */  
        UF_CALL(UF_SF_create_scenario("scenario_1", &scenario));  
        ECHO(scenario);  
        part = UF_PART_ask_display_part();  
        if ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            n = ask_body_faces(body, &faces);  
            UF_CALL(UF_SF_create_2d_mesh(n, faces, 0, NULL, &mesh_data, &mesh_id));  
            ECHO(mesh_id);  
        /*  Work around necessary to see newly created mesh  
            UF_CALL(UF_DISP_regenerate_display());  
        */  
            UF_free(faces);  
        }  
    }  
    /* 里海 */  
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

> 这段代码是一个NX Open C++ API的二次开发示例，用于创建实体体的2D网格。代码的主要功能和步骤包括：
>
> 1. 包含必要的头文件，并定义了一些宏来简化错误处理和日志输出。
> 2. 定义了一个错误处理函数report_error，用于打印函数调用失败时的错误信息。
> 3. 定义了一个函数ask_next_body，用于遍历部件中的实体体。
> 4. 定义了一个函数allocate_memory，用于在NX中分配内存。
> 5. 定义了一个函数make_an_array，用于从UF_MODL_ask_list_count返回的列表中提取实体体标签数组。
> 6. 定义了一个函数ask_body_faces，用于获取实体体的所有面。
> 7. 定义了一个函数do_it，用于实现创建2D网格的主要逻辑。首先获取当前显示的部件，然后循环遍历部件中的实体体，获取每个实体体的所有面，并使用UF_SF_create_2d_mesh接口创建2D网格。
> 8. 定义了ufusr函数，作为NX Open的二次开发程序的入口函数。在这个函数中，首先初始化NX Open环境，然后调用do_it函数，最后终止NX Open环境。
> 9. 定义了ufusr_ask_unload函数，用于在卸载二次开发程序时立即释放资源。
>
> 总的来说，这段代码通过NX Open C++ API实现了一个相对完整的创建实体体2D网格的二次开发示例，涵盖了错误处理、内存管理、实体体遍历、面查询和2D网格创建等多个方面。
>
