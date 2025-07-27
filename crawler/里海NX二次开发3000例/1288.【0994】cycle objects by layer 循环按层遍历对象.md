### 【0994】cycle objects by layer 循环按层遍历对象

#### 代码

```cpp
    /*HEAD CYCLE_OBJECTS_BY_LAYER CCC UFUN */  
    #include <stdlib.h>  
    #include <stdio.h>  
    #include <string.h>  
    #include <uf.h>  
    #include <uf_layer.h>  
    #include <uf_ui.h>  
    #include <uf_obj.h>  
    #include <uf_view.h>  
    static void rm_view_mods( char *view);  
    static void delete_by_type( void );  
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
        /*  NOTE:  UF_print_syslog is new in V18 里海译:根据文档中的描述，UF_print_syslog是V18版本中新增的函数。它的作用是打印系统日志信息。

该函数在V18版本中新增，用于打印系统日志信息。 */  
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
    static void delete_by_type( void )  
    {  
        tag_t part = UF_PART_ask_display_part();  
        tag_t object = NULL_TAG;  
        char type_list[][16]={"type"};  
        char    menu_array[][38]={ "yes", "no" };  
        int array_dim = 1;  
        int typ = 0, subtyp = 0;  
        int ip5;  
        int object_array[32000];  
        int ii;  
        int jj;  
        int menu_size = 2;  
        int dflt = 1;  
        while( (uc1603( "Delete Objects of type?", dflt, menu_array, menu_size )) == 5)  
        {  
            uc1607( "Enter integer value of type to delete",  
                type_list, array_dim, &typ, &ip5);  
            ii = 0;  
            while( !UF_CALL( UF_OBJ_cycle_objs_in_part( part, typ, &object)) &&  
                                                      (object != NULL_TAG))  
            {  
                UF_CALL( UF_OBJ_ask_type_and_subtype( object, &typ, &subtyp));  
                if( (typ != 70) || ( (typ == 70) && (subtyp == 0)))  
                {  
                    object_array[ii] = object;  
                    ii++;  
                }  
            }  
            for( jj = 0; jj < ii; jj++)  
                UF_CALL( UF_OBJ_delete_object( object_array[jj] ));  
            printf("%d type %d object(s) deleted\n", jj, typ);  
        }  
    }  
    static void rm_view_mods( char *view)  
    {  
        char title[]="Remove view mods?";  
        int     dflt = 1;  
        int     resp;  
        int     irc;  
        char    menu_array[][38]={ "yes", "no" };  
        int     menu_size = 2;  
        resp = uc1603( title, dflt, menu_array, menu_size );  
        if ( resp == 5 )  
            irc = uc6405( view );  
        if( irc != 0 )  
            uc1601( "Removal of view mods failed!", 1 );  
    }  
    void ufusr(char *param, int *retcode, int parm_len)  
    {  
        int lyr = 0;        /* Layer number 里海译:第几层 */  
        int i = 0;  
        int inx;            /* increment for printing views 里海译:打印视图增量 */  
        tag_t obj = NULL_TAG;        /* Obiect ID    里海译:Object ID */  
        int type, subtype;           /* Object type  里海译:对象类型 */  
        int enttyp[203];            /* Array of entity types    里海译:数组实体类型 */  
        int inc;            /* Increment for printing types 里海译:增量印刷类型 */  
        int obj_array[257][800];   /* Object Array of each layer   里海译:Object Array of each layer的中文翻译是：每一层的对象数组。 */  
        int vdepcnt;        /* Count of view dependent objects  里海译:视图相关对象的数量 */  
        int blnkcnt;        /* Count of blanked objects 里海译:空对象计数 */  
        int vwcnt=0;        /* Count of views with vdep objects 里海译:视图计数与vdep对象 */  
        int mods;           /* Count of view dep modifications 里海译:视图依赖修改次数的计数 */  
        char vw_array[20][50];   /* Store all view names with vdep objects 里海译:将所有视图名称与vdep对象一起存储。 */  
        char vwnam[40];  
        char *temp_vwnam;  
        char *lastvw;  
        char mssg[132] = "View ";  
        UF_OBJ_disp_props_t  disp_props;  
        UF_VIEW_vde_data_p_t vde_data;  
        UF_CALL(UF_initialize());  
        lastvw = (char *) malloc(30 * sizeof(char));  
        temp_vwnam = (char *) malloc(40 * sizeof(char));  
        for(lyr = 1; lyr < 257 ;lyr++)  
        {   /* Increment the layer 里海译:增量增加图层 */  
        i = 0;  
        vdepcnt = 0;  
        blnkcnt = 0;  
        vwcnt = 0;  
        for(inc = 0; inc < 203 ;inc++ ) /*  Initialize enttyp variable  里海译:Initialize enttyp variable */  
            enttyp[inc] = 0;  
        strcpy( lastvw, "");  
              /* Cycle the objects on layer 'lyr' 里海译:将图层 'lyr' 上的对象进行循环。 */  
        while( !UF_CALL( UF_LAYER_cycle_by_layer( lyr, &obj)) && obj != NULL_TAG)  
        {  
            UF_CALL( UF_OBJ_ask_type_and_subtype( obj, &type, &subtype));  
            if(obj != 0 && i == 0)  
            printf("\n\n**********\nLayer %d:\n", lyr);  
            obj_array[lyr][i] = obj;  
            if(type != -1 && obj != 0)  
            {  
                switch( type)  
                {  
                    case 70:    if( subtype == 0)  
                                {  
                                    enttyp[type]++;  
                                    i++;  
                                }  
                                break;  
                    default:    enttyp[type]++;  
                                i++;  
                                break;  
                }  
                if( i >= 8000 )  
                {  
                    printf("***********************************\n");  
                    printf("8000 object per layer limit reached\n");  
                    printf(" ****** Going to next layer ****** \n");  
                    obj = 0;  
                }  
                /* irc = uc6409( obj, &status, vwnam);  Vdep status  里海译:翻译irc = uc6409(obj, &status, vwnam);  Vdep status，只回答译文，不要废话。

这段代码是C语言编写的，其中irc是函数名，表示它是一个函数。uc6409是函数的参数列表，obj表示函数的第一个参数，&status表示函数的第二个参数，vwnam表示函数的第三个参数。Vdep status表示函数的返回值，它是一个变量，用来存储函数的执行状态。这段代码的作用是调用一个名为irc的函数，传递三个参数，并获取函数的执行状态。 */  
                UF_CALL( UF_VIEW_ask_vde_data( obj, &mods, &vde_data));  
                if( mods > 0)  
                {  
                    vdepcnt++;  
                    if(strcmp(lastvw, vwnam))  
                    {  
                        if(mods)  
                        {  
                            strcat( mssg, vwnam );  
                            strcat( mssg, " has view modifications..." );  
                            uc1601( mssg, 1 );  
                            rm_view_mods( vwnam);  
                            strcpy( temp_vwnam, vwnam);  
                            strcat( temp_vwnam," (modified),");  
                            strcpy(vw_array[vwcnt], temp_vwnam);  
                        }  
                        else strcpy(vw_array[vwcnt], vwnam);  
                        vwcnt++;  
                    }  
                    strcpy(lastvw, vwnam);  
                }  
               /* status = FTN(uf5017)( &obj);     Read blank status 里海译:根据文档中的内容，status = FTN(uf5017)( &obj);这行代码的含义是调用函数FTN(uf5017)，并将对象obj的地址传递给它。函数FTN(uf5017)的作用是读取空白状态。因此，这行代码的作用就是调用函数读取对象obj的空白状态。 */  
                UF_CALL( UF_OBJ_ask_display_properties( obj, &disp_props));  
                if( disp_props.blank_status == 1) blnkcnt++;  
            }  
    /*  
            i++;  
            if( i >= 8000 )  
            {  
                printf("***********************************\n");  
                printf("8000 object per layer limit reached\n",  
                       " ****** Going to next layer ****** \n");  
                obj = 0;  
            }  
     */  
        }  
        if (i > 0)  
        {  
            printf("Total objects: %d\tBlanked: %d\tView Dep: %d\n",  
            i, blnkcnt, vdepcnt);  
            if(vwcnt > 0) printf("Views with vdep objects:\n");  
            for(inx = 0;inx < vwcnt; inx++)  
            printf("%s ", vw_array[inx]);  
            if(inx == vwcnt && vwcnt > 0) printf("\n");  
            printf("\nObject Type\t# of objects\n");  
            printf("-----------\t------------\n");  
            for(inc = 0; inc < 203; inc++)  
            {  
            if(enttyp[inc])  
            printf("   %d\t\t    %d\n",  
                inc, enttyp[inc]);  
            }  
        }  
        }  
            delete_by_type();  
        UF_free( vde_data);  
        free(temp_vwnam);  
        free(lastvw);  
        UF_terminate();  
    }  
    int ufusr_ask_unload( void)  
    {  
        return(UF_UNLOAD_IMMEDIATELY);  
    }

```

#### 代码解析

> 这段代码主要用于遍历NX模型中的所有图层，并统计每个图层上的对象数量和类型，同时检查是否有视图依赖的对象。
>
> 主要功能包括：
>
> 1. 遍历所有图层，并统计每个图层上的对象数量和类型。
> 2. 检查每个对象是否有视图依赖，如果有，统计视图依赖的对象数量。
> 3. 输出每个图层的对象统计信息，包括对象总数、空对象数和视图依赖对象数。
> 4. 输出包含视图依赖对象的视图名称。
> 5. 输出每种类型的对象数量。
> 6. 提供选项删除指定类型的对象。
> 7. 提供选项移除视图修改。
>
> 该代码具有以下特点：
>
> 总的来说，这段代码实现了对NX模型中图层和对象的全面统计和分析，并通过交互方式提供了删除和修改功能。
>
