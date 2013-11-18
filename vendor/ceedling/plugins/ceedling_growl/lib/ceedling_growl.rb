require "plugin"
require "constants"
require "ruby_gntp"
#require "YAML"

module CeedlingGrowlModule

    class GrowlTypeConfig
        attr_reader :name, :title, :iconURL, :config

        def initialize(config={})
            @config = config
            @name = config[:name] || "Ceedling"
            @title = config[:title] || "Ceedling"
            @iconURL = "file:///" + File.expand_path(File.dirname __FILE__) + "/../assets/" + (config[:iconURL] || "littlesurly_crop.ico")
        end
    end

    class GrowlConfig
        attr_reader :app_name, :types

        def initialize
            @config = YAML.load_file(File.expand_path(File.dirname __FILE__) + "/../config/growl.yml")
            @app_name = @config[:app_name]
            @types = {}
            @config[:types].each do |type|
                name = type.keys.first
                @types[name] = GrowlTypeConfig.new type[name]
            end
        end
    end

    class Growl
        def initialize(name, text)
            @growlConfig = GrowlConfig.new
            @growlType = @growlConfig.types[name] || GrowlTypeConfig.new

            ::GNTP.notify({
                :app_name => @growlConfig.app_name || "Ceedling",
                :name => @growlType.name,
                :title => @growlType.title ,
                :icon => @growlType.iconURL,
                :text => text
            })
        end
    end
end

class CeedlingGrowl < Plugin
    include CeedlingGrowlModule

    def setup
        @all_passed = true
        @compiled = true
        @linked = true
    end

    def post_compile_execute(arg_hash)
        if (arg_hash[:shell_result][:exit_code] != 0)
            @all_passed = false
            @compiled = false
        end
    end

    def post_link_execute(arg_hash)
        if (arg_hash[:shell_result][:exit_code] != 0)
            @all_passed = false
            @linked = false
        end
    end

    def post_test_fixture_execute(arg_hash)
        return if not (arg_hash[:context] == TEST_SYM)
        current_file = arg_hash[:result_file]
        unless passed? current_file
            Growl.new "testFailed", testName(current_file)
            @all_passed = false
        end
    end

    def post_build
        unless @all_passed
            if @compiled and @linked
                Growl.new "testFailed", "One or more tests failed"
            else
                if @compiled == false
                    Growl.new "testFailed", "One or more files failed to compile"
                else
                    Growl.new "testFailed", "One or more runners failed to link"
                end
            end
        else
            Growl.new "testPassed", "All tests passed"
        end
    end

    private

        def testName(fileName)
            base = File.basename fileName
            ext = File.extname fileName
            base.gsub ext, ""
        end

        def passed?(fileName)
            File.extname(fileName) == ".pass"
        end
end
