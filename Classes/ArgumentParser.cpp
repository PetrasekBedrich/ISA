#include "RuntimeConfiguration.cpp"
#include <cstring>
class ArgumentParser
{
    public: 
        void Parse(int argc, char *argv[],RuntimeConfiguration* config)
        {
            config->SortBy = Bytes;
            bool interfaceFound = false;
            if(argc % 2 == 0)
            {
                fprintf(stderr,"ERROR: Invalid Number of Arguments");
                exit(1);
            }
            for(int i = 1; i<argc - 1;i++)
            {
                if(strcmp(argv[i],"-i") == 0)
                {
                    config->Interface = argv[i+1];
                    interfaceFound = true;
                    i++;
                }
                else if(strcmp(argv[i],"-s") == 0)
                {
                    if(strcmp(argv[i+1],"b") == 0)
                    {
                        config->SortBy = Bytes;
                    }
                    else if (strcmp(argv[i+1],"p") == 0)
                    {
                        config->SortBy = Packets;
                    }
                    else
                    {
                        fprintf(stderr, "ERROR: Invalid argument\n");
                        exit(1);
                    }
                    i++;
                }
                else if(strcmp(argv[i],"-c") == 0)
                {

                    config->NumberOfCommunications = atoi(argv[i+1]);
                    if (config->NumberOfCommunications  == 0) {
                        if (argv[i+1][0] != '0') {
                            fprintf(stderr, " ERROR: Number of communications has to be a valid integer");
                            exit(1);
                        }
                    }
                    i++;
                }
                else
                {
                    fprintf(stderr,"ERROR: Unknown argument");
                    exit(1);
                }
            }
            if(interfaceFound == false)
            {
                fprintf(stderr, "ERROR: Interface not found");
                exit(1);
            }
        }
};