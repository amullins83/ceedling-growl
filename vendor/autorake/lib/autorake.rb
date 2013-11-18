require File.expand_path( File.dirname __FILE__ ) + "/autotest"
require "YAML"

class Autorake
    def initialize
        @plugin_root = File.expand_path( File.dirname __FILE__ ) + "/.."
        @config = YAML.load_file(@plugin_root + "/config/autorake.yml")
        start_watching
    end

    private

        def start_watching
            AutoTest.new @config[:paths]
        end
end