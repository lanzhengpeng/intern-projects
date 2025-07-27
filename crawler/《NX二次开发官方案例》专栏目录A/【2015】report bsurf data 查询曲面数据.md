### 【2015】report bsurf data 查询曲面数据

#### 代码

```cpp
    /*HEAD REPORT_BSURF_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
    #include <uf_cfi.h>  
    #define ECHO(X)   (printf("%s = %d\n", #X, X))  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档内容，我理解的是：

V18版本新增了一个函数UF_print_syslog。该函数用于打印系统日志。 */  
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
    static tag_t ask_next_sheet_body(tag_t part, tag_t body)  
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
                if (subtype == UF_solid_body_subtype)  
                {  
                    UF_CALL(UF_MODL_ask_body_type(body, &type));  
                    if (type == UF_MODL_SHEET_BODY) return body;  
                }  
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
            err,  
            ii,  
            jj,  
            n_faces,  
            nbupol,  
            nbvpol,  
            ordu,  
            ordv,  
            uclose,  
            uperiod_st,  
            vclose,  
            vperiod_st;  
        tag_t  
            *faces,  
            part = UF_PART_ask_display_part(),  
            sheet = NULL_TAG;  
        double  
            uperiod,  
            vperiod,  
            *uknot,  
            *vknot,  
            *poles;  
        UF_MODL_bsurface_t  
            bsurf;  
        while ((sheet = ask_next_sheet_body(part, sheet)) != NULL_TAG)  
        {  
            ECHO(sheet);  
            n_faces = ask_body_faces(sheet, &faces);  
            ECHO(n_faces);  
            for (ii = 0; ii < n_faces; ii++)  
            {  
                ECHO(ii);  
                FTN(uf5464)( &faces[ii] , &nbupol, &nbvpol, &ordu, &ordv );  
                printf(" Number of pole in U direction : %d\n", nbupol );  
                printf(" Number of pole in V direction : %d\n", nbvpol );  
                printf(" Order of Face in U direction  : %d\n", ordu   );  
                printf(" Order of Face in V direction  : %d\n", ordv   );  
                uknot = (double*)UF_allocate_memory((nbupol + ordu)  
                    * sizeof(double), &err);  
                vknot = (double*)UF_allocate_memory((nbvpol + ordv)  
                    * sizeof(double), &err);  
                poles = (double*)UF_allocate_memory((nbupol * nbvpol * 4)  
                    * sizeof(double), &err);  
                FTN(uf5441)( &faces[ii] , &nbupol, &nbvpol, &ordu, &ordv,  
                    &uclose, &vclose, &uperiod_st, &uperiod ,  
                    &vperiod_st , &vperiod , uknot, vknot, poles);  
                printf("DUMP OF POLES :\n");  
                for ( jj=0 ; jj<(nbvpol*nbupol) ; jj++ )  
                    printf(" [%f %f %f - %f ]\n",  
                        poles[4*jj],poles[4*jj+1],poles[4*jj+2],poles[4*jj+3] );  
                UF_free(uknot);  
                UF_free(vknot);  
                UF_free(poles);  
                UF_CALL(UF_MODL_ask_bsurf(faces[ii], &bsurf));  
                ECHO(bsurf.num_poles_u);  
                ECHO(bsurf.num_poles_v);  
                ECHO(bsurf.order_u);  
                ECHO(bsurf.order_v);  
                ECHO(bsurf.is_rational);  
                printf("DUMP OF POLES :\n");  
                for ( jj=0 ; jj<(bsurf.num_poles_u*bsurf.num_poles_v) ; jj++ )  
                    printf(" [%f %f %f - %f ]\n", bsurf.poles[jj][0],  
                        bsurf.poles[jj][1], bsurf.poles[jj][2], bsurf.poles[jj][3]);  
                UF_free(bsurf.knots_u);  
                UF_free(bsurf.knots_v);  
                UF_free(bsurf.poles);  
            }  
            UF_free(faces);  
        }  
    }  
    /* qq3123197280 */  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        if (!UF_CALL(UF_initialize()))  
        {  
            do_it();  
            UF_CALL(UF_terminate());  
        }  
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
    /*  To run in external native mode UG:  
            program.exe /path/to/part1.prt /path/to/part2.prt [...]  
        To run in external UG/Manager or IMAN mode:  
            program.exe -pim=yes @DB/partno1/rev @DB/partno2/rev [...]  
    */  
    int main( int argc, char *argv[] )  
    {  
        int  
            cnt = 0;  
        tag_t  
            part;  
        char  
            part_name[MAX_FSPEC_SIZE+1];  
        UF_PART_load_status_t  
            status;  
        if (!UF_CALL(UF_initialize()))  
        {  
            UF_CALL(uc4624(0, argc, argv));  
            while (uc4621(part_name) == 1)  
            {  
                printf("%d.  %s\n", ++cnt, part_name);  
                if (UF_CALL(UF_PART_open(part_name, &part, &status))  
                    || status.n_parts > 0) check_load_status(&status);  
                else  
                {  
                    do_it();  
                    UF_PART_close_all();  
                }  
            }  
            printf("\nProcessed %d parts.\n", cnt);  
            UF_CALL(UF_terminate());  
        }  
        return 0;  
    }

```

#### 代码解析

> 这段NX二次开发代码的主要功能是从NX部件中提取BSURF（B样条曲面）信息，并将其打印出来。具体包括以下步骤：
>
> 1. 初始化NX环境，并调用do_it函数执行主要操作。
> 2. 在do_it函数中，首先获取当前显示的部件，并循环遍历部件中的所有片体。
> 3. 对于每个片体，获取其所有面，并遍历每个面。
> 4. 对于每个面，首先调用UF_MODL_ask_bsurf_info函数获取面的B样条曲面信息，包括U向和V向的顶点数、阶数等。
> 5. 然后调用UF_MODL_ask_bsurf函数获取面的B样条曲面详细数据，包括U向和V向的节点向量、顶点坐标等。
> 6. 打印出B样条曲面的详细信息。
> 7. 释放相关内存，继续处理下一个面。
> 8. 在主函数中，首先打开传入的部件文件，然后调用do_it函数处理部件，最后关闭所有打开的部件。
> 9. 整个过程使用了UF对象类型和模型API进行NX对象操作，并通过宏定义UF_CALL进行错误处理。
>
> 总体来说，这段代码实现了提取和打印NX部件中面的B样条曲面信息的功能。
>
