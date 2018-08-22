<%page enable_loop="True"/>

    ${cls_name}::${cls_name}()
    {
        REGISTER_MONO_NATIVE_OBJECT( ${cls_name} );
        makeMetaData();
    }