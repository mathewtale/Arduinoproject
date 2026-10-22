namespace Arduinoproject.Models
{
    public class Quizsave
    {
        public int Id { get; set; }
        public string? Question { get; set; }
        public string Answer1 { get; set; } = string.Empty;
        public string Answer2 { get; set; } = string.Empty;
        public string Answer3 { get; set; } = string.Empty;
        public string? CorrectAnswer { get; set; }
        public string? Category { get; set; }
    }
}
