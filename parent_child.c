#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

// Dosya oluşturma işlemi
void create_file() {
    FILE *file = fopen("example.txt", "w");
    if (file) {
        fprintf(file, "This is a new file.\n");
        fclose(file);
        printf("File created successfully.\n");
    } else {
        perror("Failed to create file");
        abort(); // Dosya oluşturulamazsa hata durumunda çıkış yapar.
    }
}

// Dosyaya veri ekleme işlemi
void write_file() {
    FILE *file = fopen("example.txt", "a");
    if (file) {
        fprintf(file, "This is appended text.\n");
        fclose(file);
        printf("Data written to file successfully.\n");
    } else {
        perror("Failed to write to file");
        abort(); // Dosyaya yazılamazsa hata durumunda çıkış yapar.
    }
}

// Dosyanın içeriğini okuma işlemi
void read_file() {
    execlp("cat", "cat", "example.txt", NULL); // Dosyanın içeriğini ekrana yazdırır
    perror("execlp failed"); // execlp başarısız olursa hata mesajı
    exit(1);
}

int main() {
    pid_t pid1, pid2, pid3;
    int status;

    // 1. Child process - dosya oluşturma
    pid1 = fork();
    if (pid1 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid1 == 0) {
        create_file();  // Dosya oluşturma
        exit(0);        // Child process başarılı bir şekilde sonlanır
    }

    // 2. Child process - dosyaya veri yazma
    pid2 = fork();
    if (pid2 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid2 == 0) {
        write_file();  // Dosyaya yazma
        exit(0);       // Child process başarılı bir şekilde sonlanır
    }

    // 3. Child process - dosyanın içeriğini okuma
    pid3 = fork();
    if (pid3 < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid3 == 0) {
        read_file();  // Dosyanın içeriğini okuma
        exit(0);      // Child process başarılı bir şekilde sonlanır
    }

    // Parent process tüm child'ların tamamlanmasını bekler
    waitpid(pid1, &status, 0);  // İlk child'ın bitmesini bekle
    if (WIFEXITED(status)) {
        printf("Child 1 (create_file) exited with status: %d\n", WEXITSTATUS(status));
    }

    waitpid(pid2, &status, 0);  // İkinci child'ın bitmesini bekle
    if (WIFEXITED(status)) {
        printf("Child 2 (write_file) exited with status: %d\n", WEXITSTATUS(status));
    }

    waitpid(pid3, &status, 0);  // Üçüncü child'ın bitmesini bekle
    if (WIFEXITED(status)) {
        printf("Child 3 (read_file) exited with status: %d\n", WEXITSTATUS(status));
    }

    return 0;
}
