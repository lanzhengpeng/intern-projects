### 【1496】list all body features 列出所有实体特征

#### 代码

```cpp
    /*HEAD LIST_ALL_BODY_FEATURES.CPP CCC UFUN */  
    #include <ug_exception.hxx>  
    #include <ug_session.hxx>  
    #include <ug_iterator.hxx>  
    #include <ug_body_feature.hxx>  
    #include <ug_part.hxx>  
    #include <ug_info_window.hxx>  
    #if ! defined ( __hp9000s800 ) && ! defined ( __sgi ) && ! defined ( __sun )  
    #    include <strstream>  
    #    include <iostream>  
          using std::ostrstream;  
          using std::endl;   
          using std::ends;  
          using std::cerr;  
    #else  
    #    include <strstream.h>  
    #    include <iostream.h>  
    #endif  
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
            UgBodyFeatureObject  
                *bodyFeat = dynamic_cast <UgBodyFeatureObject *> (typed_object);  
            if ( bodyFeat )  
            {  
                info.write(bodyFeat->getName());  
                info.write(" - ");  
                feat_tag = bodyFeat->getTag();  
                UF_MODL_ask_feat_type(feat_tag, &type);  
                info.write(type);  
                UF_free(type);  
                info.write("\n");  
            }  
        }  
    }  
    void ufusr(char *param, int *retcode, int paramLen)  
    {  
        UgSession my_session(true);  
        try  
        {  
            UgPart *part = my_session.getWorkPart();  
            do_it(part);  
        }  
        catch (UgException oops)  
        {  
            oops.processError();  
        }  
    }

```

#### 代码解析

> 这段代码是NX Open C++ API编写的二次开发代码，主要用于列出当前活动部件中的所有体特征及其类型。
>
> 主要功能包括：
>
> 1. 初始化NX会话，获取当前活动部件。
> 2. 遍历当前活动部件中的所有特征，并判断是否为体特征。
> 3. 如果是体特征，则获取其名称、类型等信息，并输出到NX信息窗口中。
> 4. 在遍历结束后关闭NX信息窗口。
> 5. 使用try-catch处理可能出现的异常。
>
> 代码的主要逻辑是遍历当前部件中的所有特征，判断其是否为体特征，如果是则输出其名称和类型信息。通过调用NX API来实现特征类型的获取。在遍历结束后关闭信息窗口，并使用try-catch处理可能出现的异常。
>
> 整体来说，这段代码实现了预期的功能，结构清晰，使用try-catch处理了异常，提高了程序的健壮性。
>
