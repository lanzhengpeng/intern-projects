### 【1503】list all features 列出所有特征

#### 代码

```cpp
    /*HEAD LIST_ALL_FEATURES.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_feature.hxx>  
    #include <ug_part.hxx>  
    #include <ug_info_window.hxx>  
    #include <uf.h>  
    #include <uf_ui.h>  
    #include <uf_modl.h>  
    #include <iostream.h>  
    static void do_it(UgPart *part)  
    {  
        tag_t  
            feat_tag;  
        char  
            *type;  
        UgTypedObject  
            *typed_object;  
        UgInfoWindow  
            info;  
        info.open();  
        for ( typed_object = part->iterateFirst();  
              typed_object;  
              typed_object = part->iterateNext(typed_object) )  
        {  
            UgFeatureObject  
                *feat = dynamic_cast <UgFeatureObject *> (typed_object);  
            if ( feat )  
            {  
                info.write(feat->getName());  
                info.write(" - ");  
                feat_tag = feat->getTag();  
                UF_MODL_ask_feat_type(feat_tag, &type);  
                info.write(type);  
                UF_free(type);  
                info.write("\n");  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        try  
        {  
            UgSession my_session(true);  
            UgPart *part = my_session.getWorkPart();  
            do_it(part);  
        }  
        catch (UgException oops)  
        {  
            UgInfoWindow  info;  
            info.open();  
            info.write(oops.askErrorText());  
            info.write("\n");  
            return;  
        }  
    }

```

#### 代码解析

> 这段NX Open C++代码实现了遍历当前工作部件的所有特征并显示其名称和类型的功能。
>
> 代码的主要部分如下：
>
> 1. 引入了NX Open C++的一些头文件，如ug_exception.hxx、ug_session.hxx等，这些头文件提供了NX Open C++的一些基本类和接口。
> 2. 定义了一个静态函数do_it，它接受一个UgPart指针作为参数。这个函数遍历UgPart中的所有特征对象，并获取每个特征的名称和类型，然后通过UgInfoWindow显示。
> 3. 在do_it函数中，通过UgPart的iterateFirst和iterateNext接口遍历UgPart中的所有对象，并使用dynamic_cast转换为UgFeatureObject指针。如果成功转换为特征对象，则获取其名称、类型并显示。
> 4. 定义了ufusr函数，这是NX Open C++的入口函数。在该函数中，首先创建一个UgSession对象，然后获取当前工作部件的UgPart对象，并调用do_it函数进行遍历和显示。
> 5. ufusr函数中使用了try-catch语句块来捕获UgException异常，如果发生异常则通过UgInfoWindow显示异常信息。
> 6. ufusr函数的参数符合NX Open C++的ufusr标准。
>
> 总的来说，这段代码实现了遍历当前工作部件的特征，并显示其名称和类型的功能，通过NX Open C++的UgPart、UgFeatureObject等类以及UF_MODL接口实现。
>
