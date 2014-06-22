# Set your paths. These are mine ;)
TOOLCHAIN_PATH = '~/dev/gcc-arm-none-eabi/bin/'
LM4FLASH = '~/dev/stellaris/lm4tools/lm4flash/lm4flash'
TIVAWARE_ROOT = "~/dev/stellaris/tivaware/"

TOOLCHAIN_PREFIX = 'arm-none-eabi-'
CC = TOOLCHAIN_PATH+TOOLCHAIN_PREFIX+'gcc'
LD = TOOLCHAIN_PATH+TOOLCHAIN_PREFIX+'ld' # Not used, replaced with GCC
OBJCOPY = TOOLCHAIN_PATH+TOOLCHAIN_PREFIX+'objcopy'

BASELIBS = [ TIVAWARE_ROOT + '/driverlib/gcc/libdriver.a' ]

PART = 'TM4C1233H6PM'
VARIANT = 'cm4f'
CPU = '-mcpu=cortex-m4'
FPU = '-mfpu=fpv4-sp-d16 -mfloat-abi=softfp'

AFLAGS = [
  '-mthumb',
  CPU,
  FPU,
  '-MD',
]

CFLAGS = [
  '-g', '-O0', # For now prefer debug versions
  '-mthumb',
  CPU,
  FPU,
  '-ffunction-sections',
  '-fdata-sections',
  '-std=c99',
  '-Wall',
  '-pedantic',
  '-DPART_'+PART,
  '-DTARGET_IS_BLIZZARD_RA1',
]

LD_TEMPLATE = 'lm4f120h5qr.ld'

LDFLAGS = [
  '--gc-sections',
]

FIRMWARE = 'obj/firmware.bin'
DEBUGFIRMWARE = 'obj/firmware.axf'

OBJDIR = 'obj'

INC_DIRS = FileList['**/inc/']
INC_DIRS << TIVAWARE_ROOT

C_FILES = FileList['**/src/*.c']
H_FILES = FileList['**/inc/*.h']

CFLAGS.concat INC_DIRS.collect {|d| "-I "+d }

# Get the location of libgcc.a from the GCC front-end.
BASELIBS << `#{CC} #{CFLAGS.join ' '} -print-libgcc-file-name`.chomp

# Get the location of libc.a from the GCC front-end.
BASELIBS << `#{CC} #{CFLAGS.join ' '} -print-file-name=libc.a`.chomp

# Get the location of libm.a from the GCC front-end.
BASELIBS << `#{CC} #{CFLAGS.join ' '} -print-file-name=libm.a`.chomp

# Get the location of libnosys.a from the GCC front-end.
BASELIBS << `#{CC} #{CFLAGS.join ' '} -print-file-name=libnosys.a`.chomp

# Add tasks for each c source file
OBJ_FILES = C_FILES.collect { |fn|
  ofile = File.join(OBJDIR, File.basename(fn).ext('o'))
  file ofile => [OBJDIR, fn] do
    command = "#{CC} #{CFLAGS.join ' '} -o #{ofile} -c #{fn}"
    puts command
    sh command
  end
  ofile
}

directory OBJDIR

file DEBUGFIRMWARE => OBJ_FILES do |t|
  sh "#{CC} #{CFLAGS.join ' '} #{LDFLAGS.map{|f| "-Wl,#{f}"}.join ' '} -T #{LD_TEMPLATE} -o #{t.name} #{OBJ_FILES.join ' '} #{BASELIBS.join ' '}"
end

file FIRMWARE => [DEBUGFIRMWARE] do |t|
  sh "#{OBJCOPY} -O binary #{DEBUGFIRMWARE} #{t.name}"
end

task :default => [FIRMWARE]

task :flash => [FIRMWARE] do |t|
  sh "#{LM4FLASH} -v #{FIRMWARE}"
end

task :clean do |t|
  sh "rm -rf #{OBJDIR}"
end
