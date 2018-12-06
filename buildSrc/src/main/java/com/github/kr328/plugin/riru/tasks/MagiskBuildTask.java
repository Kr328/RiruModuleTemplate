package com.github.kr328.plugin.riru.tasks;

import com.github.kr328.plugin.riru.exts.MagiskExtension;
import com.github.kr328.plugin.riru.utils.PathUtils;
import org.gradle.api.DefaultTask;
import org.gradle.api.GradleScriptException;
import org.gradle.api.tasks.TaskAction;

import java.io.*;
import java.util.Map;
import java.util.Objects;
import java.util.zip.ZipEntry;
import java.util.zip.ZipOutputStream;

public class MagiskBuildTask extends DefaultTask {
    public MagiskExtension extension;

    @TaskAction
    @SuppressWarnings({"unused", "ResultOfMethodCallIgnored"})
    public void run() throws IOException {
        File outputFile = new File(PathUtils.toLocalSeparator(getProject().getBuildDir().getAbsolutePath() + "/output/magisk-module.zip"));
        outputFile.getParentFile().mkdirs();

        ZipOutputStream outputStream = new ZipOutputStream(new FileOutputStream(outputFile));

        for ( Map.Entry<String ,String> entry : extension.getZipMap().entrySet() ) {
            archiveEntry(entry.getKey() ,entry.getValue() ,outputStream);
        }

        outputStream.close();
    }

    private void archiveEntry(String source ,String target ,ZipOutputStream stream) throws IOException {
        File sourceFile = new File(source);

        if ( sourceFile.isDirectory() ) {
            for ( File f : Objects.requireNonNull(sourceFile.listFiles()))
                archiveEntry(f.getAbsolutePath() ,target + "/" + f.getName() ,stream);
        }
        else if ( sourceFile.isFile() ) {
            ZipEntry entry = new ZipEntry(PathUtils.trimPathSeparator(target));
            stream.putNextEntry(entry);
            readFileToStream(new File(source) ,stream);
        }
        else
            throw new GradleScriptException("Magisk file not found." ,new FileNotFoundException(source));
    }

    @SuppressWarnings("ResultOfMethodCallIgnored")
    private void readFileToStream(File file , OutputStream stream) throws IOException {
        FileInputStream inputStream = new FileInputStream(file);
        byte[] buffer = new byte[inputStream.available()];
        inputStream.read(buffer);
        stream.write(buffer);
        inputStream.close();
    }
}
