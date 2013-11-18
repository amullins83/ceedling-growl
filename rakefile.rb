PROJECT_CEEDLING_ROOT = "vendor/ceedling"
PROJECT_AUTORAKE_ROOT = "vendor/autorake"
load "#{PROJECT_CEEDLING_ROOT}/lib/rakefile.rb"
load "#{PROJECT_AUTORAKE_ROOT}/lib/autorake.rb"


task :default => %w[ test:all release convert2hex copy2trunk]

task :convert2hex do
    system("pic32-bin2hex build/release/M9DP.elf")
end

task :copy2trunk do
    system("cp build/release/M9DP.hex firmware.hex")
end

task :auto do
    Autorake.new
end
