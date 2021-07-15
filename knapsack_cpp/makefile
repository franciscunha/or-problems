MAIN = knapsack

CPLEX_VERSION = 12.10
SYSTEM  = x86-64_linux
BITS_OPTION = -m64
LIBFORMAT = static_pic
CPPC = g++

#### libs do cplex
CPLEXDIR  = /opt/ibm/ILOG/CPLEX_Studio1210/cplex
CONCERTDIR = /opt/ibm/ILOG/CPLEX_Studio1210/concert
CPLEXLIBDIR   = $(CPLEXDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
CONCERTLIBDIR = $(CONCERTDIR)/lib/$(SYSTEM)/$(LIBFORMAT)
#############################

#### opcoes de compilacao e includes
CCOPT = $(BITS_OPTION) -fPIC -fexceptions -DNDEBUG -DIL_STD -std=c++11 -fpermissive -Wno-ignored-attributes
DBFLAG = -O3 
CONCERTINCDIR = $(CONCERTDIR)/include
CPLEXINCDIR   = $(CPLEXDIR)/include
CCFLAGS = $(CCOPT) -I$(CPLEXINCDIR) -I$(CONCERTINCDIR)
#############################

#### flags do linker
CCLNFLAGS = -L$(CPLEXLIBDIR) -lilocplex -lcplex -L$(CONCERTLIBDIR) -lconcert -lm -lpthread -ldl -Wl,--no-as-needed 
#############################

#### diretorios com os source files e com os objs files
SRCDIR = src
OBJDIR = obj
#############################

#### lista de todos os srcs e todos os objs
SRCS = $(wildcard $(SRCDIR)/*.cpp)
OBJS = $(patsubst $(SRCDIR)/%.cpp, $(OBJDIR)/%.o, $(SRCS))
#############################

#### regra principal, gera o executavel
$(MAIN): $(OBJS) 
	@echo  "\033[31m \nLinking all objects files: \033[0m"
	$(CPPC) $(BITS_OPTION) $(OBJS) -o $@ $(CCLNFLAGS)
############################

# substitui flag de otimizacao por flag de debug
debug: DBFLAG = -g3
debug: $(MAIN)

# inclui os arquivos de dependencias
-include $(OBJS:.o=.d)

# regra para cada arquivo objeto: compila e gera o arquivo de dependencias do arquivo objeto
# cada arquivo objeto depende do .c e dos headers (informacao dos header esta no arquivo de 
# dependencias gerado pelo compiler)
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@echo  "\033[31m \nCompiling $<: \033[0m"
	$(CPPC) $(DBFLAG) $(CCFLAGS) -c $< -o $@
	@echo  "\033[32m \ncreating $< dependency file: \033[0m"
	$(CPPC) -std=c++11  -MM $< > $(basename $@).d
# proximas tres linhas colocam o diretorio no arquivo de dependencias (g++ nao coloca, surprisingly!)
	@mv -f $(basename $@).d $(basename $@).d.tmp 
	@sed -e 's|.*:|$(basename $@).o:|' < $(basename $@).d.tmp > $(basename $@).d
	@rm -f $(basename $@).d.tmp

# deleta objetos e arquivos de dependencia
clean:
	@echo "\033[31mcleaning obj directory \033[0m"
	@rm -f $(OBJDIR)/*.o $(OBJDIR)/*.d
	@rm $(MAIN)

rebuild: clean $(MAIN)


