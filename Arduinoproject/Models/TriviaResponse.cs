using System.Text.Json.Serialization;

namespace Arduinoproject.Models
{
    public class TriviaResponse
    {
        [JsonPropertyName("category")]
        public string? Category { get; set; }

        [JsonPropertyName("question")]
        public string? Question { get; set; }

        [JsonPropertyName("correct_answer")]
        public string? CorrectAnswer { get; set; }

        [JsonPropertyName("incorrect_answers")]
        public List<string> IncorrectAnswers { get; set; } = new();
    }
}
