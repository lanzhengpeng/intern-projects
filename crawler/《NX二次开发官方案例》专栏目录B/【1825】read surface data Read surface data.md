### 【1825】read surface data Read surface data

#### 代码

```cpp
    /*HEAD READ_SURFACE_DATA CCC UFUN */  
    #include <stdio.h>  
    #include <stdlib.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_object_types.h>  
    #include <uf_part.h>  
    #include <uf_modl.h>  
    #include <uf_obj.h>  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:UF_print_syslog是在V18版本中新增的。 */  
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
    static tag_t get_next_face(tag_t face)  
    {  
        int  
            subtype,  
            type;  
        tag_t  
            part = UF_PART_ask_display_part();  
        do  
        {  
            UF_CALL(UF_OBJ_cycle_objs_in_part(part, UF_solid_type, &face));  
            if (face)  
            {  
                UF_CALL(UF_OBJ_ask_type_and_subtype(face, &type, &subtype));  
                if (subtype == UF_solid_face_subtype) return face;  
            }  
        } while (face);  
        return FALSE;  
    }  
    static void do_it(void)  
    {  
        double  
            uperiod,  
            vperiod,  
            *uknot,  
            *vknot,  
            *poles;  
        int  
            ii,  
            nbupol,  
            nbvpol,  
            ordu,  
            ordv,  
            uclose,  
            uperiod_st,  
            vclose,  
            vperiod_st;  
        tag_t  
            face = NULL_TAG,  
            sheet;  
        while ((face = get_next_face(face)) != NULL_TAG)  
        {  
            UF_CALL(UF_MODL_extract_face(face, 2, &sheet));  
            FTN(uf5464)( &sheet , &nbupol, &nbvpol, &ordu, &ordv );  
            printf(" Number of pole in U direction : %d\n", nbupol );  
            printf(" Number of pole in V direction : %d\n", nbvpol );  
            printf(" Order of Face in U direction  : %d\n", ordu   );  
            printf(" Order of Face in V direction  : %d\n", ordv   );  
            uknot = (double*)malloc( (nbupol + ordu )* sizeof( double) );  
            vknot = (double*)malloc( (nbvpol + ordv )* sizeof( double) );  
            poles = (double*)malloc( (nbupol*nbvpol*4 )* sizeof( double) );  
            FTN(uf5441)( &sheet , &nbupol, &nbvpol, &ordu, &ordv,  
                &uclose, &vclose, &uperiod_st, &uperiod ,  
                &vperiod_st , &vperiod , uknot, vknot, poles);  
            printf("DUMP OF POLES :\n");  
            for ( ii=0 ; ii<(nbvpol*nbupol) ; ii++ )  
                printf(" [%f %f %f - %f ]\n",  
                    poles[4*ii],poles[4*ii+1],poles[4*ii+2],poles[4*ii+3] );  
            free(uknot);  
            free(vknot);  
            free(poles);  
            UF_CALL(UF_OBJ_delete_object(sheet));  
        }  
    }  
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

```

#### 代码解析

> 这段代码是一个NX的二次开发代码，主要实现了以下功能：
>
> 1. 定义了错误报告函数report_error，用于在出错时打印错误信息。
> 2. 定义了获取下一个面的函数get_next_face，用于遍历零件中的所有面。
> 3. 定义了主函数do_it，用于遍历零件中的所有面，提取面的参数信息（包括极点数、次数、周期性等），并打印出来。
> 4. 定义了ufusr函数，作为NX的二次开发入口函数，初始化UF模块，调用do_it函数，然后终止UF模块。
> 5. 定义了ufusr_ask_unload函数，用于设置卸载二次开发模块的方式。
>
> 整体来看，这段代码实现了遍历零件所有面，并提取每个面的参数化信息的功能。
>
