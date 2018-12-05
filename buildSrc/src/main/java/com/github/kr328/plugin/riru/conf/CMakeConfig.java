package com.github.kr328.plugin.riru.conf;

import org.gradle.api.GradleScriptException;

import java.util.Properties;

public class CMakeConfig {
    public String cmakePath;
    public String androidNdkPath;

    public static CMakeConfig read() {
        Properties  properties = LocalProperties.get();
        CMakeConfig result = new CMakeConfig();

        result.cmakePath = properties.getProperty("cmake.bin");
        result.androidNdkPath = properties.getProperty("android.ndk");

        if ( result.cmakePath == null )
            throw new GradleScriptException("cmake.bin must set in local.properties" ,new Exception());

        if ( result.androidNdkPath == null )
            throw new GradleScriptException("android.ndk must set in local.properties" ,new Exception());

        return result;
    }
}
