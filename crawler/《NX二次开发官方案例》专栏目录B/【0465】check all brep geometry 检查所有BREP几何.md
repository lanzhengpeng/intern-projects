### 【0465】check all brep geometry 检查所有BREP几何

#### 代码

```cpp
    /*HEAD CHECK_ALL_BREP_GEOMETRY CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_object_types.h>  
    #include <uf_disp.h>  
    #include <uf_brep.h>  
    #include <uf_part.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 译:根据文档内容，UF_print_syslog是在V18版本中新增的。在翻译时，我们只需要回答UF_print_syslog在V18版本中是新增的，不需要添加其他无关信息。 */  
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
    static void check_all_brep_geometry(UF_BREP_topology_t *child_topology,  
        int *n_bad)  
    {  
        int  
            ii,  
            nstates,  
            unique_id;  
        char  
            name[MAX_ENTITY_NAME_SIZE+1];  
        UF_BREP_geometry_t  
            geometry;  
        UF_BREP_orientation_t  
            geom_sense;  
        UF_BREP_state_t  
            *states;  
        UF_BREP_ask_identifier(child_topology, &unique_id);  
        if (child_topology->type > UF_BREP_SHELL_TOPO)  
        {  
            if (UF_CALL(UF_BREP_ask_geometry(child_topology, NULL, NULL, &geometry,  
                &geom_sense, &nstates, &states)))  
            {  
                ++(*n_bad);  
                switch (child_topology->type)  
                {  
                    case UF_BREP_FACE_TOPO:  
                        printf ("Face");  
                        break;  
                    case UF_BREP_LOOP_TOPO:  
                        printf ("Loop");  
                        break;  
                    case UF_BREP_FIN_TOPO:  
                        printf ("Fin");  
                        break;  
                    case UF_BREP_EDGE_TOPO:  
                        printf ("Edge");  
                        break;  
                    case UF_BREP_VERTEX_TOPO:  
                        printf ("Vertex");  
                        break;  
                    default:  
                        printf ("Unknown");  
                        break;  
                }  
                if (child_topology->tag != NULL_TAG)  
                {  
                    sprintf(name, "BAD_%d", *n_bad);  
                    printf(":  UGtag=%d, id=%d, name=%s\n", child_topology->tag,  
                        unique_id, name);  
                    UF_CALL(UF_OBJ_set_name(child_topology->tag, name));  
                }  
                else  
                    printf(":  UGtag=%d, id=%d\n", child_topology->tag, unique_id);  
            }  
            else  
            {  
                if (nstates > 0) UF_free(states);  
                UF_CALL(UF_BREP_free_geometry_data(&geometry, NULL));  
            }  
        }  
        for (ii = 0; ii < child_topology->num_children; ii++)  
            check_all_brep_geometry(child_topology->children[ii].child, n_bad);  
    }  
    static void do_it(int *n_bad)  
    {  
        int  
            nstates;  
        tag_t  
            body = NULL_TAG,  
            part = UF_PART_ask_display_part();  
        UF_BREP_topology_p_t  
            topo;  
        UF_BREP_state_p_t  
            states;  
        while ((body = ask_next_body(part, body)) != NULL_TAG)  
        {  
            UF_CALL(UF_BREP_ask_topology(body, 0, &topo, &nstates, &states));  
            check_all_brep_geometry(topo, n_bad);  
            if (nstates > 0) UF_free(states);  
            UF_CALL(UF_BREP_release_topology(topo, NULL));  
        }  
    }  
    /* 里海 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        int  
            n_bad = 0;  
        if (UF_CALL(UF_initialize())) return;  
        do_it(&n_bad);  
        printf("Found %d errors\n", n_bad);  
        UF_terminate();  
    }  
    int ufusr_ask_unload(void)  
    {  
        return (UF_UNLOAD_IMMEDIATELY);  
    }  
    static void check_load_status(UF_PART_load_status_p_t status)  
    {  
        char  
            msg[133];  
        int  
            ii;  
        for (ii=0; ii<status->n_parts; ii++)  
        {  
            UF_get_fail_message(status->statuses[ii], msg);  
            printf("    %s - %s\n", status->file_names[ii], msg);  
        }  
        UF_free(status->statuses);  
        UF_free_string_array(status->n_parts, status->file_names);  
    }  
    static void build_copy_partname(char *copy_fspec)  
    {  
        char  
            dspec[UF_CFI_MAX_PATH_NAME_SIZE],  
            part_name[UF_CFI_MAX_FILE_NAME_SIZE],  
            part_fspec[UF_CFI_MAX_PATH_NAME_SIZE];  
        tag_t  
            part = UF_PART_ask_display_part();  
        UF_PART_ask_part_name(part, part_fspec);  
        UF_CALL(uc4576(part_fspec, 2, dspec, part_name));  
        UF_CALL(uc4578(part_name, 2, part_name));  
        strcat(part_name, "-new");  
        UF_CALL(uc4575(dspec, 2, part_name, copy_fspec));  
    }  
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0,  
            n_bad;  
        tag_t  
            part;  
        char  
            new_fspec[MAX_FSPEC_SIZE+1],  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                UF_CALL(UF_PART_open(part_name, &part, &status));  
                if (status.n_parts > 0) check_load_status(&status);  
                if (!status.failed)  
                {  
                    n_bad = 0;  
                    do_it(&n_bad);  
                    printf("Found %d errors\n", n_bad);  
                    if (n_bad > 0)  
                    {  
                        build_copy_partname(new_fspec);  
                        if (!UF_CALL(UF_PART_save_as(new_fspec)))  
                            printf("\tsaved as %s\n", new_fspec);  
                    }  
                    UF_CALL(UF_PART_close_all());  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码的功能是检查NX部件文件中的几何体错误，并报告任何检测到的错误。具体来说，代码的主要逻辑如下：
>
> 1. 初始化NX环境。
> 2. 循环处理每个提供的部件文件名：打开部件文件。遍历部件中的所有实体，并检查其几何体信息。如果发现几何体错误，将错误信息打印到控制台，并将错误实体重命名以便识别。如果检测到错误，将部件另存为新文件。关闭部件文件。
> 3. 打开部件文件。
> 4. 遍历部件中的所有实体，并检查其几何体信息。
> 5. 如果发现几何体错误，将错误信息打印到控制台，并将错误实体重命名以便识别。
> 6. 如果检测到错误，将部件另存为新文件。
> 7. 关闭部件文件。
> 8. 统计处理的总部件数量。
> 9. 终止NX环境。
>
> 此外，代码中使用了NX提供的UF函数来遍历部件的实体、查询几何体信息、报告错误等。错误处理函数会打印错误代码和消息，并重命名错误实体。在主函数中，代码可以接受文件路径或数据库零件编号作为参数，支持外部模式和内部模式两种运行方式。
>
> 总体来说，这段代码实现了对NX部件文件中几何错误的检查和报告功能。
>
