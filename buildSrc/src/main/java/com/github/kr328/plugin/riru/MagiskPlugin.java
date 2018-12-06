package com.github.kr328.plugin.riru;

import com.github.kr328.plugin.riru.exts.MagiskExtension;
import com.github.kr328.plugin.riru.tasks.CMakeBuildTask;
import com.github.kr328.plugin.riru.tasks.MagiskBuildTask;
import com.github.kr328.plugin.riru.tasks.TransformDexTask;
import org.gradle.api.Plugin;
import org.gradle.api.Project;
import org.gradle.api.Task;

public class MagiskPlugin implements Plugin<Project> {
    @Override
    public void apply(Project target) {
        MagiskExtension extension = target.getExtensions().create("magisk" , MagiskExtension.class);
        MagiskBuildTask task = target.getTasks().create("buildMagiskModule" , MagiskBuildTask.class);

        //task.mustRunAfter(TransformDexTask.class , CMakeBuildTask.class);
        task.mustRunAfter("transformDex" ,"cmakeBuild");

        task.extension = extension;
    }
}
